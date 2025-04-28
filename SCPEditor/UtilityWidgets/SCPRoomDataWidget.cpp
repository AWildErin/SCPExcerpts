#include "UtilityWidgets/SCPRoomDataWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "DesktopPlatformModule.h"
#include "EditorUtilityLibrary.h"
#include "EditorUtilityWidgetComponents.h"
#include "Engine/StaticMeshActor.h"
#include "Logging/StructuredLog.h"
#include "Subsystems/EditorAssetSubsystem.h"

#include "Map/SCPRoomDataAsset.h"
#include "SCPEditorModule.h"
#include "SCPEditorSettings.h"

#define LogDebug(format, ...) UE_LOGFMT(LogSCPEditor, Log, "[{funcsig}] " format, FString(__FUNCTION__), ##__VA_ARGS__)
#define LogWarning(format, ...) UE_LOGFMT(LogSCPEditor, Warning, "[{funcsig}] " format, FString(__FUNCTION__), ##__VA_ARGS__)
#define LogError(format, ...) UE_LOGFMT(LogSCPEditor, Error, "[{funcsig}] " format, FString(__FUNCTION__), ##__VA_ARGS__)

/** How much to shrink the bounding box by */
constexpr float BOX_SHRINK_AMOUNT = 0.05f;

void USCPRoomDataWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	RoomIniPath->SetText(FText::FromString("F:/Random Games/SCP/SCP - Containment Breach v1.3.11/Data/rooms.ini"));
}

void USCPRoomDataWidget::GatherDataAssets()
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> AssetData;
	AssetRegistry.GetAssetsByClass(FTopLevelAssetPath(USCPRoomDataAsset::StaticClass()->GetPathName()), AssetData);

	for (auto& Asset : AssetData)
	{
		USCPRoomDataAsset* DataAsset = Cast<USCPRoomDataAsset>(Asset.GetAsset());
		if (!DataAsset)
		{
			LogError("Failed to cast to USCPRoomDataAsset");
			continue;
		}

		RoomAssets.Add(DataAsset, Asset);
	}
}

void USCPRoomDataWidget::AssignRoomIniValues()
{
	if (RoomIniPath->GetText().IsEmpty())
	{
		LogError("RoomIniPath was empty!!");
		return;
	}

	TObjectPtr<UEditorAssetSubsystem> AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();

	FString IniPath = RoomIniPath->GetText().ToString();
	const USCPEditorSettings* Settings = USCPEditorSettings::Get();

	// int Index = 31; // 914
	// auto& DataAssetEntry = RoomAssets.Array()[Index];
	for (auto& DataAssetEntry : RoomAssets)
	{
		USCPRoomDataAsset* DataAsset = DataAssetEntry.Key;
		FAssetData& AssetData = DataAssetEntry.Value;
		FString ObjectPath = AssetData.GetObjectPathString();

		FString RoomName = DataAsset->RoomName;

		if (Settings->RoomAlias.Contains(DataAsset))
		{
			RoomName = Settings->RoomAlias[DataAsset];
		}

		if (!GConfig->DoesSectionExist(*RoomName, IniPath))
		{
			LogWarning("Failed to find section for {name} in {ini}", RoomName, IniPath);
			continue;
		}

		// When we update a value via the DataAsset, it doesn't actually fully update it
		bool bChangedSomething = false;

		// Commoness
		int Commonness = GConfig->GetIntOrDefault(*RoomName, TEXT("commonness"), 0, IniPath);
		if (DataAsset->Commonness != Commonness)
		{
			DataAsset->Commonness = Commonness;
			LogDebug("Set {da}'s commonness to be {val}", AssetData.AssetName.ToString(), Commonness);
			bChangedSomething = true;
		}

		// Overlap check
		bool bDisableOverlap = GConfig->GetBoolOrDefault(*RoomName, TEXT("disableoverlapcheck"), false, IniPath);
		if (DataAsset->bDisableOverlapCheck != bDisableOverlap)
		{
			DataAsset->bDisableOverlapCheck = bDisableOverlap;
			LogDebug("Set {da}'s overlap check to be {val}", AssetData.AssetName.ToString(), bDisableOverlap);
			bChangedSomething = true;
		}

		// Set shape
		FString RoomShape = GConfig->GetStringOrDefault(*RoomName, TEXT("shape"), "1", IniPath);

		// Check the shape, then check if we actually need to update the shape
		if (RoomShape == "1")
		{
			if (DataAsset->RoomType != ESCPRoomType::EndRoom)
			{
				DataAsset->RoomType = ESCPRoomType::EndRoom;
				bChangedSomething = true;
				UE_LOGFMT(LogSCPEditor, Log, "Set {0}'s RoomType to be {1}", AssetData.AssetName.ToString(), RoomShape);
			}
		}
		else if (RoomShape == "2")
		{
			if (DataAsset->RoomType != ESCPRoomType::TwoWay)
			{
				DataAsset->RoomType = ESCPRoomType::TwoWay;
				bChangedSomething = true;
				UE_LOGFMT(LogSCPEditor, Log, "Set {0}'s RoomType to be {1}", AssetData.AssetName.ToString(), RoomShape);
			}
		}
		else if ((RoomShape == "2C" || RoomShape == "2c"))
		{
			if (DataAsset->RoomType != ESCPRoomType::TwoWayCorner)
			{
				DataAsset->RoomType = ESCPRoomType::TwoWayCorner;
				bChangedSomething = true;
				UE_LOGFMT(LogSCPEditor, Log, "Set {0}'s RoomType to be {1}", AssetData.AssetName.ToString(), RoomShape);
			}
		}
		else if (RoomShape == "3")
		{
			if (DataAsset->RoomType != ESCPRoomType::ThreeWay)
			{
				DataAsset->RoomType = ESCPRoomType::ThreeWay;
				bChangedSomething = true;
				UE_LOGFMT(LogSCPEditor, Log, "Set {0}'s RoomType to be {1}", AssetData.AssetName.ToString(), RoomShape);
			}
		}
		else if (RoomShape == "4")
		{
			if (DataAsset->RoomType != ESCPRoomType::FourWay)
			{
				DataAsset->RoomType = ESCPRoomType::FourWay;
				bChangedSomething = true;
				UE_LOGFMT(LogSCPEditor, Log, "Set {0}'s RoomType to be {1}", AssetData.AssetName.ToString(), RoomShape);
			}
		}
		else
		{
			UE_LOGFMT(LogSCPEditor, Warning, "Unknown room shape for {0}", RoomName);
		}

		// Set Zones
		int Zone0 = GConfig->GetIntOrDefault(*RoomName, TEXT("zone0"), 0, IniPath); // Not valid for us, but we should warn the user about that. CB has this
																					// which is valid for some reason, not sure why.
		int Zone1 = GConfig->GetIntOrDefault(*RoomName, TEXT("zone1"), 0, IniPath);
		int Zone2 = GConfig->GetIntOrDefault(*RoomName, TEXT("zone2"), 0, IniPath);
		int Zone3 = GConfig->GetIntOrDefault(*RoomName, TEXT("zone3"), 0, IniPath);

		if (Zone0 > 0)
		{
			LogError(
				"Section for %s has a non zero value for zone0. We do not support that here. Please manually check what the zone is and apply it accordingly.", *RoomName);
		}

		if ((int)DataAsset->RoomZone.Zone1 != Zone1)
		{
			DataAsset->RoomZone.Zone1 = (ESCPRoomZone)Zone1;
			LogDebug("Set {da}'s zone 1 to be {val}", AssetData.AssetName.ToString(), Zone1);
			bChangedSomething = true;
		}

		if ((int)DataAsset->RoomZone.Zone2 != Zone2)
		{
			DataAsset->RoomZone.Zone2 = (ESCPRoomZone)Zone2;
			LogDebug("Set {da}'s zone 2 to be {val}", AssetData.AssetName.ToString(), Zone2);
			bChangedSomething = true;
		}

		if ((int)DataAsset->RoomZone.Zone3 != Zone3)
		{
			DataAsset->RoomZone.Zone3 = (ESCPRoomZone)Zone3;
			LogDebug("Set {da}'s zone 1 to be {val}", AssetData.AssetName.ToString(), Zone3);
			bChangedSomething = true;
		}

		if (bChangedSomething)
		{
			if (!AssetSubsystem->SaveAsset(ObjectPath, false))
			{
				LogError("Failed to save {path}!", ObjectPath);
				continue;
			}
		}
	}
}

void USCPRoomDataWidget::GenerateRoomExtents()
{
	TObjectPtr<UEditorAssetSubsystem> AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();

	//	int Index = 31; // 914
	// auto& DataAssetEntry = RoomAssets.Array()[Index];
	for (auto& DataAssetEntry : RoomAssets)
	{
		USCPRoomDataAsset* DataAsset = DataAssetEntry.Key;
		FAssetData& AssetData = DataAssetEntry.Value;
		FString ObjectPath = AssetData.GetObjectPathString();

		UWorld* RoomWorld = DataAsset->RoomLevel.LoadSynchronous();
		if (RoomWorld == nullptr)
		{
			LogError("{da}'s RoomLevel was null!", DataAsset->GetName());
			continue;
		}

		// Our room generator creates a mesh with this exact name, so we use that to get the mesh.
		FName ActorName = FName("RoomStaticMesh");

		// Find the static mesh
		/** @todo Could we maybe do this better... this really isn't ideal :( */
		auto Pred = [&](AActor* ArrayItem) { return ArrayItem->GetFName() == ActorName; };
		auto Value = RoomWorld->PersistentLevel->Actors.FindByPredicate(Pred);
		if (Value == nullptr)
		{
			LogError("{da}'s RoomLevel did not contain \"RoomMeshActor\"!", DataAsset->GetName());
			continue;
		}

		/** @todo Why did I do this? */
		TObjectPtr<AStaticMeshActor> Actor = Cast<AStaticMeshActor>(*RoomWorld->PersistentLevel->Actors.FindByPredicate(Pred));
		if (Actor == nullptr || Actor->GetStaticMeshComponent()->GetStaticMesh() == nullptr)
		{
			LogError("{da}'s RoomLevel did not contain \"RoomMeshActor\"!", DataAsset->GetName());
			continue;
		}

		UStaticMesh* Mesh = Actor->GetStaticMeshComponent()->GetStaticMesh();

		FBoxSphereBounds BoxBounds = Mesh->GetBounds();
		FBox Box = BoxBounds.GetBox();

		// Construct the new box bounds like SCP:CB does
		FVector Min;
		Min.X = Box.Min.X + BOX_SHRINK_AMOUNT;
		Min.Y = Box.Min.Y + BOX_SHRINK_AMOUNT;
		Min.Z = Box.Min.Z + BOX_SHRINK_AMOUNT;

		FVector Max;
		Max.X = Box.Max.X - BOX_SHRINK_AMOUNT;
		Max.Y = Box.Max.Y - BOX_SHRINK_AMOUNT;
		Max.Z = Box.Max.Z - BOX_SHRINK_AMOUNT;

		// Check if we actually need to update them
		if (DataAsset->ExtentsMin != Min || DataAsset->ExtentsMax != Max)
		{
			DataAsset->ExtentsMin = Min;
			DataAsset->ExtentsMax = Max;

			if (!AssetSubsystem->SaveAsset(ObjectPath, false))
			{
				LogError("Failed to save {path}!", ObjectPath);
				continue;
			}
		}
	}
}

FString USCPRoomDataWidget::SelectIniFile()
{
	void* ParentWindowHandle = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();

	static FString PreviousDirectory = FPaths::ProjectDir();

	TArray<FString> FileNames;
	bool bOpen = FDesktopPlatformModule::Get()->OpenFileDialog(ParentWindowHandle, "Select rooms.ini file", PreviousDirectory, "", "Room Data File|rooms.ini",
															   EFileDialogFlags::Multiple, FileNames);
	if (bOpen)
	{
		if (FileNames.Num() > 0)
		{
			// Set the directory.
			PreviousDirectory = FPaths::GetPath(FileNames[0]);

			return FileNames[0];
		}
		else
		{
			LogError("Please pick a file!!!");
		}
	}

	return FString();
}
