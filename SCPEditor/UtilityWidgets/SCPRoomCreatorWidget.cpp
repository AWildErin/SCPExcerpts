#include "UtilityWidgets/SCPRoomCreatorWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "Components/BrushComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/TextBlock.h"
#include "DesktopPlatformModule.h"
#include "EditorDialogLibrary.h"
#include "EditorLevelUtils.h"
#include "EditorUtilityLibrary.h"
#include "EditorUtilityWidgetComponents.h"
#include "Engine/BlockingVolume.h"
#include "Engine/Brush.h"
#include "Engine/Note.h"
#include "Engine/Pointlight.h"
#include "Engine/Polys.h"
#include "Engine/Spotlight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TriggerVolume.h"
#include "EngineUtils.h"
#include "Factories/DataAssetFactory.h"
#include "FileHelpers.h"
#include "Logging/StructuredLog.h"
#include "Subsystems/EditorAssetSubsystem.h"
#include "UObject/SavePackage.h"

#include "Factories/RMeshFactory.h"
#include "SCPEditorModule.h"
#include "SCPEditorSettings.h"
#include "cbreader/rmesh.h"

#define LogDebug(format, ...) UE_LOGFMT(LogSCPEditor, Log, "[{funcsig}] " format, FString(__FUNCTION__), ##__VA_ARGS__)
#define LogWarning(format, ...) UE_LOGFMT(LogSCPEditor, Warning, "[{funcsig}] " format, FString(__FUNCTION__), ##__VA_ARGS__)
#define LogError(format, ...) UE_LOGFMT(LogSCPEditor, Error, "[{funcsig}] " format, FString(__FUNCTION__), ##__VA_ARGS__)

constexpr float ROOM_SCALE = 0.6f;

static const FName RoomStaticMeshName = FName(TEXT("RoomStaticMesh"));

void USCPRoomCreatorWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (BaseBPOutputPath)
	{
		BaseBPOutputPath->SetText(FText::FromString(TEXT("/Game/SCPCB/Blueprints/Map/Rooms/LCZ")));
	}

	if (BaseMeshOutputPath)
	{
		BaseMeshOutputPath->SetText(FText::FromString(TEXT("/Game/SCPCB/Art/Map/Rooms/LCZ")));
	}

	DataAssetClass = USCPRoomDataAsset::StaticClass();

	bDoAssetCheck = false;
	bHaltOnFailure = false;

	if (DoAssetCheckCheckBox && HaltOnFailCheckBox)
	{
		DoAssetCheckCheckBox->SetIsChecked(bDoAssetCheck);
		HaltOnFailCheckBox->SetIsChecked(bHaltOnFailure);
	}

	// Defaults
	bDefaultGenerateLevel = true;
	bDefaultOverrideLevel = false;
	bDefaultGenerateMesh = true;
	bDefaultGenerateDataAsset = true;

	if (DefaultGenerateLevelCheckBox && DefaultOverrideLevelCheckBox && DefaultGenerateMeshCheckBox && DefaultGenerateDataAssetCheckBox)
	{
		DefaultGenerateLevelCheckBox->SetIsChecked(bDefaultGenerateLevel);
		DefaultOverrideLevelCheckBox->SetIsChecked(bDefaultOverrideLevel);
		DefaultGenerateMeshCheckBox->SetIsChecked(bDefaultGenerateMesh);
		DefaultGenerateDataAssetCheckBox->SetIsChecked(bDefaultGenerateDataAsset);
	}
}

TArray<FString> USCPRoomCreatorWidget::SelectRMeshFiles()
{
	void* ParentWindowHandle = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();

	static FString PreviousDirectory = FPaths::ProjectDir();

	TArray<FString> FileNames;
	bool bOpen = FDesktopPlatformModule::Get()->OpenFileDialog(ParentWindowHandle, "Select .rmesh file", PreviousDirectory, "", "Room Mesh File|*.rmesh",
															   EFileDialogFlags::Multiple, FileNames);
	if (bOpen)
	{
		if (FileNames.Num() > 0)
		{
			// Set the directory.
			PreviousDirectory = FPaths::GetPath(FileNames[0]);

			return FileNames;
		}
		else
		{
			LogError("Please pick a file!!!");
		}
	}

	return TArray<FString>();
}

bool USCPRoomCreatorWidget::AddFilesToList(TArray<FString> FilePaths)
{
	if (BaseBPOutputPath->GetText().IsEmpty())
	{
		UEditorDialogLibrary::ShowMessage(FText::FromString("Invalid BP base path!"), FText::FromString("Base BP output path was empty! Please fill it."), EAppMsgType::Ok);
		return false;
	}

	if (BaseMeshOutputPath->GetText().IsEmpty())
	{
		UEditorDialogLibrary::ShowMessage(FText::FromString("Invalid mesh base path!"), FText::FromString("Base mesh output path was empty! Please fill it."),
										  EAppMsgType::Ok);
		return false;
	}

	for (auto& FilePath : FilePaths)
	{
		FString BaseFileName = FPaths::GetBaseFilename(FilePath);
		FString BaseNameWithoutPrefix = BaseFileName.Replace(TEXT("SM_"), TEXT(""));

		TObjectPtr<USCPRoomCreatorData> Data = NewObject<USCPRoomCreatorData>(this);
		Data->RoomName = BaseNameWithoutPrefix;
		Data->MeshName = BaseFileName;

		Data->BPOutputPath = BaseBPOutputPath->GetText().ToString();
		Data->MeshOutputPath = BaseMeshOutputPath->GetText().ToString();

		if (bAppendNameToArtPath)
		{
			Data->MeshOutputPath = Data->MeshOutputPath / BaseNameWithoutPrefix;
		}

		if (bAppendNameToBPPath)
		{
			Data->BPOutputPath = Data->BPOutputPath / BaseNameWithoutPrefix;
		}

		Data->bGenerateLevel = bDefaultGenerateLevel;
		Data->bOverrideLevel = bDefaultOverrideLevel;
		Data->bGenerateMesh = bDefaultGenerateMesh;
		Data->bGenerateDataAsset = bDefaultGenerateDataAsset;

		Data->RMeshPath = FilePath;

		ListView->AddItem(Data);
	}

	return true;
}

void USCPRoomCreatorWidget::ClearList()
{
	ListView->ClearListItems();
}

void USCPRoomCreatorWidget::GenerateAllRooms()
{
	TArray<FString> Warnings;
	TArray<FString> Errors;
	int FailedImportCount = 0;

	for (auto& Entry : ListView->GetListItems())
	{
		TObjectPtr<USCPRoomCreatorData> Data = Cast<USCPRoomCreatorData>(Entry);

		if (Data->bGenerateMesh && !ImportStaticMesh(Data))
		{
			FailedImportCount++;
			if (bHaltOnFailure)
			{
				break;
			}
			else
			{
				continue;
			}
		}

		if (Data->bGenerateLevel && !GenerateRoomLevel(Data))
		{
			FailedImportCount++;
			if (bHaltOnFailure)
			{
				break;
			}
			else
			{
				continue;
			}
		}

		if (Data->bGenerateDataAsset && !GenerateRoomAsset(Data))
		{
			FailedImportCount++;
			if (bHaltOnFailure)
			{
				break;
			}
			else
			{
				continue;
			}
		}
	}
}

bool USCPRoomCreatorWidget::ImportStaticMesh(TObjectPtr<USCPRoomCreatorData> Data)
{
	LogDebug("Generate Static Mesh for {name}", Data->RoomName);

	FString PackageName = Data->MeshOutputPath / Data->MeshName;

	// Converted package path for SavePackage. It has to be done this way for some reason
	FString FullPackagePath = UPackageTools::PackageNameToFilename(PackageName, ".uasset");

	UPackage* Package = CreatePackage(*PackageName);
	Package->FullyLoad();
	Package->Modify();

	TObjectPtr<URMeshFactory> Factory = NewObject<URMeshFactory>();
	bool bTemp;
	UStaticMesh* StaticMesh = Cast<UStaticMesh>(Factory->FactoryCreateFile(UStaticMesh::StaticClass(), Package, FName(Data->MeshName),
																		   RF_Public | RF_Standalone, Data->RMeshPath, nullptr, GWarn, bTemp));
	if (!StaticMesh)
	{
		LogError("Failed to create static mesh for {name}", Data->RoomName);
		return false;
	}

	// Assign materials
	const USCPEditorSettings* EditorSettings = USCPEditorSettings::Get();
	TArray<FStaticMaterial>& Materials = StaticMesh->GetStaticMaterials();
	for (auto& Material : Materials)
	{
		TSoftObjectPtr<UMaterialInterface> MaterialInterface = EditorSettings->RMeshMaterialMap.FindRef(Material.MaterialSlotName.ToString());

		if (MaterialInterface.IsPending() || MaterialInterface.IsValid())
		{
			// LoadSynchronous doesn't try to load the material if it's already loaded, instead returns the pointer to the object.
			Material.MaterialInterface = MaterialInterface.LoadSynchronous();
		}
		else
		{
			LogError(TEXT("{material} did not exist on the material map!"), Material.MaterialSlotName.ToString());
		}
	}

	StaticMesh->SetStaticMaterials(Materials);

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.Error = GError;
	SaveArgs.bWarnOfLongFilename = false;
	SaveArgs.SaveFlags = SAVE_None;

	TObjectPtr<UEditorAssetSubsystem> AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!UPackage::SavePackage(Package, StaticMesh, *FullPackagePath, SaveArgs) || !AssetSubsystem->SaveAsset(PackageName, false))
	{
		return false;
	}

	Package->PostEditChange();
	FAssetRegistryModule::AssetCreated(Package);

	return true;
}

bool USCPRoomCreatorWidget::GenerateRoomAsset(TObjectPtr<USCPRoomCreatorData> Data)
{
	LogDebug("Generate Data Asset for {name}", Data->RoomName);

	if (Data->RoomType == ESCPRoomType::None)
	{
		LogError("Invalid room type for {name}!", Data->RoomName);
		return false;
	}

	// if (!Data->RoomZone.IsValid())
	//{
	//	LogError("Invalid room zone for {name}!", *Data->RoomName);
	//	return false;
	// }

	const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& AssetTools = AssetToolsModule.Get();
	TObjectPtr<UEditorAssetSubsystem> AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();

	FString DataAssetName = TEXT("DA_") + Data->RoomName;

	// if (bDoAssetCheck && AssetSubsystem->DoesAssetExist(Data->BPOutputPath / DataAssetName))
	//{
	//	bool bShouldContinue = ThrowYesNoMessage("Data asset already exists!", FString::Printf(TEXT("A data asset at %s/%s already exists. Continuing will "
	//																								"overwite this asset. Are you sure you want to continue?"),
	//																						   *Data->BPOutputPath, *DataAssetName));
	//	if (!bShouldContinue)
	//	{
	//		LogWarning("Chose not to continue exporting {name}!", *Data->RoomName);
	//		return false;
	//	}
	// }

	TObjectPtr<UDataAssetFactory> DataAssetFactory = NewObject<UDataAssetFactory>();
	DataAssetFactory->DataAssetClass = DataAssetClass;

	TObjectPtr<USCPRoomDataAsset> DataAsset = Cast<USCPRoomDataAsset>(AssetTools.CreateAsset(DataAssetName, Data->BPOutputPath, UDataAsset::StaticClass(), DataAssetFactory));
	if (!DataAsset)
	{
		LogError(TEXT("Failed to create DataAsset at {path}/{name}"), Data->BPOutputPath, DataAssetName);
		return false;
	}

	FString RoomPath = FString::Printf(TEXT("%s/%s.%s"), *Data->BPOutputPath, *Data->RoomName, *Data->RoomName);

	DataAsset->RoomName = Data->RoomName.ToLower();
	DataAsset->RoomType = Data->RoomType;
	DataAsset->RoomZone = Data->RoomZone;
	DataAsset->RoomLevel = FSoftObjectPath(RoomPath);
	DataAsset->bSpecialRoom = Data->bSpecialRoom;

	return true;
}

TObjectPtr<AActor> USCPRoomCreatorWidget::FindActorByName(TObjectPtr<UWorld> World, FName Name)
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_SCPFindActorByName_Pred);

	auto Pred = [&](AActor* ArrayItem) { return ArrayItem->GetFName() == Name; };
	auto Value = World->PersistentLevel->Actors.FindByPredicate(Pred);

	return *Value;
}

bool USCPRoomCreatorWidget::GenerateRoomLevel(TObjectPtr<USCPRoomCreatorData> Data)
{
	LogDebug("Generate level for {name}", Data->RoomName);

	if (GWorld->GetOutermost()->IsDirty())
	{
		bool bShouldContinue =
			ThrowYesNoMessage("Save current map!", "The current map opened has unsaved changes. Continuing will risk losing data! Are you sure?");
		if (!bShouldContinue)
		{
			return false;
		}
	}

	/** @todo maybe make these class variables so we don't get the per room asset */
	TObjectPtr<UEditorAssetSubsystem> AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& AssetTools = AssetToolsModule.Get();

	FString RoomPath = Data->BPOutputPath / Data->RoomName;
	// Converted package path for SaveMap. It has to be done this way for some reason
	FString FullPackagePath = UPackageTools::PackageNameToFilename(RoomPath, FPackageName::GetMapPackageExtension());

	if (bDoAssetCheck && AssetSubsystem->DoesAssetExist(RoomPath))
	{
		bool bShouldContinue = ThrowYesNoMessage(
			"Level already exists!",
			FString::Printf(TEXT("A level at %s already exists. Continuing will overwite this asset. Are you sure you want to continue?"), *RoomPath));

		if (!bShouldContinue)
		{
			LogWarning("Chose not to continue exporting {name}!", Data->RoomName);
			return false;
		}
	}

	rmesh::RMesh File;
	bool bReadMesh = File.Read(TCHAR_TO_UTF8(*Data->RMeshPath));
	if (!bReadMesh)
	{
		LogError("Failed to read RMesh File ({path}). No actors will be imported.", Data->RMeshPath);
		return false;
	}

	bool bReturn = false;
	// If the asset already exists, update it
	if (AssetSubsystem->DoesAssetExist(RoomPath) && !Data->bOverrideLevel)
	{
		TObjectPtr<UWorld> World = Cast<UWorld>(AssetSubsystem->LoadAsset(RoomPath));
		checkf(World, TEXT("%s was not a UWorld!"), *FullPackagePath);

		// Check if the world is not currently loaded, if it isn't try to load it
		if (GWorld != World && !FEditorFileUtils::LoadMap(FullPackagePath, true, false))
		{
			LogError("Failed to open existing level! ({path})", FullPackagePath);
			return false;
		}

		bReturn = ImportExistingLevel(File, bReadMesh, GWorld, Data);
	}
	// If not, then we import a new level from the template
	else
	{
		FString RoomTemplate = "/Game/Editor/Template";
		if (!FEditorFileUtils::LoadMap(RoomTemplate, true, false))
		{
			LogError("Failed to open template level! ({path})", RoomTemplate);
			return false;
		}

		bReturn = ImportFreshLevel(File, bReadMesh, GWorld, Data);
	}

	// Save the world
	if (bReturn)
	{
		if (!FEditorFileUtils::SaveMap(GWorld, FullPackagePath) || !AssetSubsystem->SaveAsset(RoomPath, false))
		{
			LogError("Failed to save map! ({path})", RoomPath);
			return false;
		}
	}

	return bReturn;
}

bool USCPRoomCreatorWidget::ImportFreshLevel(rmesh::RMesh& File, bool bRMeshValid, TObjectPtr<UWorld> World, TObjectPtr<USCPRoomCreatorData> Data)
{
	TObjectPtr<UEditorAssetSubsystem> AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();

	if (bRMeshValid)
	{
		ImportRMeshEntities(File, World);

		// Add collision boxes
		if (File.collisionMesh.surfaces.size() > 0)
		{
			ImportRMeshCollisions(File, World);
		}

		// Add trigger boxes
		if (File.triggerBoxes.size() > 0)
		{
			ImportRMeshTriggers(File, World);
		}
	}

	FString MeshPath = Data->MeshOutputPath / Data->MeshName;
	if (AssetSubsystem->DoesAssetExist(MeshPath))
	{
		TObjectPtr<UStaticMesh> StaticMesh = Cast<UStaticMesh>(AssetSubsystem->LoadAsset(MeshPath));

		if (StaticMesh)
		{
			FActorSpawnParameters Params;
			Params.Name = RoomStaticMeshName;
			TObjectPtr<AStaticMeshActor> Actor = World->SpawnActor<AStaticMeshActor>(Params);
			Actor->SetActorLabel("RoomStaticMesh");
			Actor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
		}
		else
		{
			LogError("Failed to cast to static mesh using {path}", MeshPath);
			return false;
		}
	}
	else
	{
		LogWarning("Static mesh did not exist {path}", MeshPath);
	}

	return true;
}

bool USCPRoomCreatorWidget::ImportExistingLevel(rmesh::RMesh& File, bool bRMeshValid, TObjectPtr<UWorld> World, TObjectPtr<USCPRoomCreatorData> Data)
{
	TObjectPtr<UEditorAssetSubsystem> AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();

	FString MeshPath = Data->MeshOutputPath / Data->MeshName;

	// Check if we want to import the mesh entity
	if (Data->HasImportFlag(ESCPRoomCreatorFlags::RoomMesh) && AssetSubsystem->DoesAssetExist(MeshPath))
	{
		auto Pred = [&](AActor* ArrayItem) { return ArrayItem->GetFName() == RoomStaticMeshName; };
		auto Value = World->PersistentLevel->Actors.FindByPredicate(Pred);

		if (Value == nullptr)
		{
			LogError("Couldn't find static mesh actor with name \"{name}\"", RoomStaticMeshName.ToString());
			return false;
		}

		AStaticMeshActor* Actor = Cast<AStaticMeshActor>(*Value);
		TObjectPtr<UStaticMesh> StaticMesh = Cast<UStaticMesh>(AssetSubsystem->LoadAsset(MeshPath));

		if (Actor == nullptr)
		{
			LogError("Couldn't find static mesh actor with name \"{name}\"", RoomStaticMeshName.ToString());
			return false;
		}

		if (StaticMesh == nullptr)
		{
			LogError("Couldn't find static mesh at path \"{path}\"", MeshPath);
			return false;
		}

		if (Actor->GetStaticMeshComponent()->GetStaticMesh() != StaticMesh)
		{
			Actor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
		}
	}

	if (Data->HasImportFlag(ESCPRoomCreatorFlags::Screens))
	{
		int EntityNumber = 0;
		for (auto& Screen : File.screenEntities)
		{
			FString ActorName = FString::Printf(TEXT("ScreenEntity_%d"), EntityNumber);
			FName ActorFName = FName(ActorName);
			EntityNumber++;

			TObjectPtr<AActor> Actor = FindActorByName(World, ActorFName);
			if (Actor != nullptr && Actor->IsA<ANote>())
			{
				UpdateScreenEntity(Screen, Cast<ANote>(Actor), World);
			}
		}
	}

	if (Data->HasImportFlag(ESCPRoomCreatorFlags::SoundEmitters))
	{
		int EntityNumber = 0;
		for (auto& SoundEmitter : File.soundEmitterEntities)
		{
			FString ActorName = FString::Printf(TEXT("SoundEmitterEntity_%d"), EntityNumber);
			FName ActorFName = FName(ActorName);
			EntityNumber++;

			TObjectPtr<AActor> Actor = FindActorByName(World, ActorFName);
			if (Actor != nullptr && Actor->IsA<ANote>())
			{
				UpdateSoundEmitterEntity(SoundEmitter, Cast<ANote>(Actor), World);
			}
		}
	}

	if (Data->HasImportFlag(ESCPRoomCreatorFlags::SpotLights))
	{
		int EntityNumber = 0;
		for (auto& SpotLight : File.spotlightEntities)
		{
			FString ActorName = FString::Printf(TEXT("SpotLightEntity_%d"), EntityNumber);
			FName ActorFName = FName(ActorName);
			EntityNumber++;

			TObjectPtr<AActor> Actor = FindActorByName(World, ActorFName);
			if (Actor != nullptr && Actor->IsA<ASpotLight>())
			{
				UpdateSpotLightEntity(SpotLight, Cast<ASpotLight>(Actor), World);
			}
		}
	}

	if (Data->HasImportFlag(ESCPRoomCreatorFlags::PointLights))
	{
		int EntityNumber = 0;
		for (auto& Light : File.lightEntities)
		{
			FString ActorName = FString::Printf(TEXT("LightEntity_%d"), EntityNumber);
			FName ActorFName = FName(ActorName);
			EntityNumber++;

			TObjectPtr<AActor> Actor = FindActorByName(World, ActorFName);
			if (Actor != nullptr && Actor->IsA<APointLight>())
			{
				UpdatePointLightEntity(Light, Cast<APointLight>(Actor), World);
			}
		}
	}

	if (Data->HasImportFlag(ESCPRoomCreatorFlags::Models))
	{
		int EntityNumber = 0;
		for (auto& Model : File.modelEntities)
		{
			FString ActorName = FString::Printf(TEXT("ModelEntity_%d"), EntityNumber);
			FName ActorFName = FName(ActorName);
			EntityNumber++;

			TObjectPtr<AActor> Actor = FindActorByName(World, ActorFName);
			if (Actor != nullptr && Actor->IsA<ANote>())
			{
				UpdateModelEntity(Model, Cast<ANote>(Actor), World);
			}
		}
	}

	return true;
}

void USCPRoomCreatorWidget::ImportRMeshEntities(rmesh::RMesh& File, TObjectPtr<UWorld> World)
{
	int ScreenNumber = 0;
	for (auto& ScreenEntity : File.screenEntities)
	{
		cbtypes::Vector3 Position = ScreenEntity.position;
		cbtypes::Vector3 Rotation = ScreenEntity.rotation;

		FTransform Transform;
		Transform.SetTranslation(FVector(Position.z * ROOM_SCALE, Position.x * ROOM_SCALE, Position.y * ROOM_SCALE));

		FString ActorName = FString::Printf(TEXT("ScreenEntity_%d"), ScreenNumber);
		FActorSpawnParameters Params;
		Params.Name = FName(ActorName);
		ScreenNumber++;

		TObjectPtr<ANote> Note = World->SpawnActor<ANote>(ANote::StaticClass(), Transform, Params);
		Note->SetActorLabel(ActorName);
		UpdateScreenEntity(ScreenEntity, Note, World);
	}

	int SoundEmitterNumber = 0;
	for (auto& SoundEmitter : File.soundEmitterEntities)
	{
		cbtypes::Vector3 Position = SoundEmitter.position;
		cbtypes::Vector3 Rotation = SoundEmitter.rotation;

		FTransform Transform;
		Transform.SetTranslation(FVector(Position.z * ROOM_SCALE, Position.x * ROOM_SCALE, Position.y * ROOM_SCALE));
		Transform.SetRotation(FRotator(Rotation.x, Rotation.y, Rotation.z).Quaternion());

		FString ActorName = FString::Printf(TEXT("SoundEmitterEntity_%d"), SoundEmitterNumber);
		FActorSpawnParameters Params;
		Params.Name = FName(ActorName);
		SoundEmitterNumber++;

		TObjectPtr<ANote> Note = World->SpawnActor<ANote>(ANote::StaticClass(), Transform, Params);
		Note->SetActorLabel(ActorName);
		UpdateSoundEmitterEntity(SoundEmitter, Note, World);
	}

	int SpotLightNumber = 0;
	for (auto& SpotLight : File.spotlightEntities)
	{
		cbtypes::Vector3 Position = SpotLight.position;

		FTransform Transform;
		Transform.SetTranslation(FVector(Position.z * ROOM_SCALE, Position.x * ROOM_SCALE, Position.y * ROOM_SCALE));

		FString ActorName = FString::Printf(TEXT("SpotLightEntity_%d"), SpotLightNumber);
		FActorSpawnParameters Params;
		Params.Name = FName(ActorName);
		SpotLightNumber++;

		TObjectPtr<ASpotLight> SpotLightActor = World->SpawnActor<ASpotLight>(ASpotLight ::StaticClass(), Transform, Params);
		SpotLightActor->SetActorLabel(ActorName);
		UpdateSpotLightEntity(SpotLight, SpotLightActor, World);
	}

	int LightNumber = 0;
	for (auto& Light : File.lightEntities)
	{
		cbtypes::Vector3 Position = Light.position;

		FTransform Transform;
		Transform.SetTranslation(FVector(Position.z * ROOM_SCALE, Position.x * ROOM_SCALE, Position.y * ROOM_SCALE));

		FString ActorName = FString::Printf(TEXT("LightEntity_%d"), LightNumber);
		FActorSpawnParameters Params;
		Params.Name = FName(ActorName);
		LightNumber++;

		TObjectPtr<APointLight> PointLightActor = World->SpawnActor<APointLight>(APointLight::StaticClass(), Transform, Params);
		PointLightActor->SetActorLabel(ActorName);
		UpdatePointLightEntity(Light, PointLightActor, World);
	}

	int ModelNumber = 0;
	for (auto& Model : File.modelEntities)
	{
		cbtypes::Vector3 Position = Model.position;
		cbtypes::Vector3 Rotation = Model.rotation;
		cbtypes::Vector3 Scale = Model.scale;

		FTransform Transform;
		Transform.SetTranslation(FVector(Position.z * ROOM_SCALE, Position.x * ROOM_SCALE, Position.y * ROOM_SCALE));

		FString ActorName = FString::Printf(TEXT("ModelEntity_%d"), ModelNumber);
		FActorSpawnParameters Params;
		Params.Name = FName(ActorName);
		ModelNumber++;

		TObjectPtr<ANote> Note = World->SpawnActor<ANote>(ANote::StaticClass(), Transform, Params);
		Note->SetActorLabel(ActorName);
	}
}

void USCPRoomCreatorWidget::ImportRMeshCollisions(rmesh::RMesh& File, TObjectPtr<UWorld> World)
{
	// RMesh splits surfaces by textures, so there's only really ever 1 surface for collisions
	rmesh::Mesh& CollisionMesh = File.collisionMesh;
	rmesh::Surface& Surface = CollisionMesh.surfaces[0];

	if (CollisionMesh.surfaces.size() > 1)
	{
		LogWarning("Collision mesh has multiple surfaces!");
	}

	TObjectPtr<ABlockingVolume> Volume = World->SpawnActor<ABlockingVolume>();
	Volume->PreEditChange(nullptr);

	Volume->PolyFlags = 0;
	Volume->Brush = NewObject<UModel>(Volume, NAME_None, RF_Transactional);
	Volume->Brush->Initialize(nullptr, true);
	Volume->Brush->Polys = NewObject<UPolys>(Volume->Brush, NAME_None, RF_Transactional);
	Volume->GetBrushComponent()->Brush = Volume->Brush;

	for (int i = 0; i < Surface.triangles.size(); i++)
	{
		cbtypes::Triangle& Triangle = Surface.triangles[i];

		FPoly Poly;
		Poly.Init();
		Poly.iLink = i / 3;

		int Index1 = Triangle.index3;
		int Index2 = Triangle.index2;
		int Index3 = Triangle.index1;

		rmesh::Vertex Vert1 = Surface.vertices[Index1];
		rmesh::Vertex Vert2 = Surface.vertices[Index2];
		rmesh::Vertex Vert3 = Surface.vertices[Index3];

		// Add the verts back in a reversed order so we generate the correct normals
		Poly.Vertices.Emplace(FVector(Vert3.vertex.z * ROOM_SCALE, Vert3.vertex.x * ROOM_SCALE, Vert3.vertex.y * ROOM_SCALE));
		Poly.Vertices.Emplace(FVector(Vert2.vertex.z * ROOM_SCALE, Vert2.vertex.x * ROOM_SCALE, Vert2.vertex.y * ROOM_SCALE));
		Poly.Vertices.Emplace(FVector(Vert1.vertex.z * ROOM_SCALE, Vert1.vertex.x * ROOM_SCALE, Vert1.vertex.y * ROOM_SCALE));

		Poly.CalcNormal(1);

		int FinalizeReturn = Poly.Finalize(Volume, 1);
		if (FinalizeReturn == 0)
		{
			Volume->Brush->Polys->Element.Add(Poly);
		}
		else
		{
			LogError("Failed to finalize collision poly! {num}", FinalizeReturn);
		}
	}

	Volume->Brush->Linked = 1;
	Volume->GetBrushComponent()->Mobility = EComponentMobility::Static;
	Volume->Brush->Modify();
	Volume->Brush->BuildBound();
	Volume->PostEditImport();
	Volume->PostEditChange();
	Volume->RegisterAllComponents();
}

void USCPRoomCreatorWidget::ImportRMeshTriggers(rmesh::RMesh& File, TObjectPtr<UWorld> World)
{
	for (auto& TriggerBox : File.triggerBoxes)
	{
		rmesh::Surface& Surface = TriggerBox.surfaces[0];

		if (TriggerBox.surfaces.size() > 1)
		{
			LogWarning("Trigger box {name} has multiple surfaces!", FString(TriggerBox.name.c_str()));
		}

		FString TriggerName = FString(UTF8_TO_TCHAR(TriggerBox.name.c_str()));

		FActorSpawnParameters Params;
		Params.Name = FName(TriggerName);
		Params.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

		TObjectPtr<ATriggerVolume> Volume = World->SpawnActor<ATriggerVolume>(Params);
		Volume->SetActorLabel(TriggerName);

		Volume->PreEditChange(nullptr);

		Volume->PolyFlags = 0;
		Volume->Brush = NewObject<UModel>(Volume, NAME_None, RF_Transactional);
		Volume->Brush->Initialize(nullptr, true);
		Volume->Brush->Polys = NewObject<UPolys>(Volume->Brush, NAME_None, RF_Transactional);
		Volume->GetBrushComponent()->Brush = Volume->Brush;

		for (int i = 0; i < Surface.triangles.size(); i++)
		{
			cbtypes::Triangle& Triangle = Surface.triangles[i];

			FPoly Poly;
			Poly.Init();
			Poly.iLink = i / 3;

			int Index1 = Triangle.index3;
			int Index2 = Triangle.index2;
			int Index3 = Triangle.index1;

			rmesh::Vertex Vert1 = Surface.vertices[Index1];
			rmesh::Vertex Vert2 = Surface.vertices[Index2];
			rmesh::Vertex Vert3 = Surface.vertices[Index3];

			// Add the verts back in a reversed order so we generate the correct normals
			Poly.Vertices.Emplace(FVector(Vert3.vertex.z * ROOM_SCALE, Vert3.vertex.x * ROOM_SCALE, Vert3.vertex.y * ROOM_SCALE));
			Poly.Vertices.Emplace(FVector(Vert2.vertex.z * ROOM_SCALE, Vert2.vertex.x * ROOM_SCALE, Vert2.vertex.y * ROOM_SCALE));
			Poly.Vertices.Emplace(FVector(Vert1.vertex.z * ROOM_SCALE, Vert1.vertex.x * ROOM_SCALE, Vert1.vertex.y * ROOM_SCALE));

			Poly.CalcNormal(1);

			int FinalizeReturn = Poly.Finalize(Volume, 1);
			if (FinalizeReturn == 0)
			{
				Volume->Brush->Polys->Element.Add(Poly);
			}
			else
			{
				LogError("Failed to finalize collision poly! {num}", FinalizeReturn);
			}
		}

		Volume->Brush->Linked = 1;
		Volume->GetBrushComponent()->Mobility = EComponentMobility::Static;
		Volume->Brush->Modify();
		Volume->Brush->BuildBound();
		Volume->PostEditImport();
		Volume->PostEditChange();
		Volume->RegisterAllComponents();

		Volume->SetFolderPath("Triggers");
	}
}

void USCPRoomCreatorWidget::UpdateScreenEntity(rmesh::EntityScreen& Screen, TObjectPtr<ANote> Actor, TObjectPtr<UWorld> World)
{
	if (!Actor || !World)
	{
		return;
	}

	Actor->Text = FString::Printf(TEXT("className: %s\nimgPath: %s"), UTF8_TO_TCHAR(Screen.className.c_str()), UTF8_TO_TCHAR(Screen.imgPath.c_str()));
	Actor->SetFolderPath("Screen Entities");
}

void USCPRoomCreatorWidget::UpdateSoundEmitterEntity(rmesh::EntitySoundEmitter& SoundEmitter, TObjectPtr<ANote> Actor, TObjectPtr<UWorld> World)
{
	if (!Actor || !World)
	{
		return;
	}

	Actor->Text = FString::Printf(TEXT("className: %s\nsound: %d, range: %f"), UTF8_TO_TCHAR(SoundEmitter.className.c_str()), SoundEmitter.sound, SoundEmitter.range);
	Actor->SetFolderPath("Sound Emitters");
}

void USCPRoomCreatorWidget::UpdateSpotLightEntity(rmesh::EntitySpotLight& SpotLight, TObjectPtr<ASpotLight> Actor, TObjectPtr<UWorld> World)
{
	if (!Actor || !World)
	{
		return;
	}

	Actor->SetMobility(EComponentMobility::Stationary);
	Actor->SpotLightComponent->InnerConeAngle = SpotLight.innerConeAngle;
	Actor->SpotLightComponent->OuterConeAngle = SpotLight.outerConeAngle;
	Actor->SpotLightComponent->AttenuationRadius = SpotLight.range;

	FString RotationString = UTF8_TO_TCHAR(SpotLight.angles.c_str());

	TArray<FString> RotationArray;
	RotationString.ParseIntoArray(RotationArray, TEXT(" "));

	float Pitch = FCString::Atof(*RotationArray[0]);
	float Yaw = FCString::Atof(*RotationArray[1]);
	float Roll = FCString::Atof(*RotationArray[2]);

	// Can't set the rotation using the transform for some reason
	Actor->SetActorRotation(FRotator(-Pitch, Yaw, Roll));

	// Convert the color because it's in a string for some reason
	FString LightColorString = UTF8_TO_TCHAR(SpotLight.color.c_str());

	TArray<FString> LightColorArray;
	LightColorString.ParseIntoArray(LightColorArray, TEXT(" "));

	int R = FCString::Atoi(*LightColorArray[0]);
	int G = FCString::Atoi(*LightColorArray[1]);
	int B = FCString::Atoi(*LightColorArray[2]);

	Actor->SpotLightComponent->LightColor = FColor(R, G, B);
	// 2500 is a generally good baseline after doing some tests. It is half of the original starting value in UE of 5000
	Actor->SpotLightComponent->Intensity = (2500 * SpotLight.intensity);
	Actor->PostEditChange();

	Actor->SetFolderPath("Spot Lights");
}

void USCPRoomCreatorWidget::UpdatePointLightEntity(rmesh::EntityLight& Light, TObjectPtr<APointLight> Actor, TObjectPtr<UWorld> World)
{
	if (!Actor || !World)
	{
		return;
	}

	Actor->SetMobility(EComponentMobility::Stationary);
	Actor->PointLightComponent->AttenuationRadius = Light.range;

	// Convert the color because it's in a string for some reason
	FString LightColorString = UTF8_TO_TCHAR(Light.color.c_str());

	TArray<FString> LightColorArray;
	LightColorString.ParseIntoArray(LightColorArray, TEXT(" "));

	int R = FCString::Atoi(*LightColorArray[0]);
	int G = FCString::Atoi(*LightColorArray[1]);
	int B = FCString::Atoi(*LightColorArray[2]);

	Actor->PointLightComponent->LightColor = FColor(R, G, B);
	// 2500 is a generally good baseline after doing some tests. It is half of the original starting value in UE of 5000
	Actor->PointLightComponent->Intensity = (2500 * Light.intensity);
	Actor->PostEditChange();

	Actor->SetFolderPath("Point Lights");
}

void USCPRoomCreatorWidget::UpdateModelEntity(rmesh::EntityModel& Model, TObjectPtr<ANote> Actor, TObjectPtr<UWorld> World)
{
	if (!Actor || !World)
	{
		return;
	}

	cbtypes::Vector3 Position = Model.position;
	cbtypes::Vector3 Rotation = Model.rotation;
	cbtypes::Vector3 Scale = Model.scale;

	FString Text = FString::Printf(TEXT("model %s\npos: %f %f %f\nrot: %f %f %f\nscale: %f %f %f"), UTF8_TO_TCHAR(Model.path.c_str()), Position.x, Position.y,
								   Position.z, Rotation.x, Rotation.y, Rotation.z, Scale.x, Scale.y, Scale.z);
	Actor->Text = Text;
	Actor->SetFolderPath("Models");
}

bool USCPRoomCreatorWidget::ThrowYesNoMessage(FString Title, FString Body)
{
	TEnumAsByte<EAppReturnType::Type> ReturnValue = UEditorDialogLibrary::ShowMessage(FText::FromString(Title), FText::FromString(Body), EAppMsgType::YesNo);

	return ReturnValue != EAppReturnType::No;
}
