#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "Map/SCPRoomDataAsset.h"

#include "SCPRoomCreatorWidget.generated.h"

class UEditorUtilityComboBoxString;
class UEditorUtilityEditableTextBox;
class UEditorUtilityCheckBox;
class UEditorUtilityButton;
class UEditorUtilityListView;
class UTextBlock;

class UDataAssetFactory;
class IAssetTools;

class ANote;
class AStaticMeshActor;
class ASpotLight;
class APointLight;

namespace rmesh
{
	struct RMesh;
	struct EntityScreen;
	struct EntitySoundEmitter;
	struct EntitySpotLight;
	struct EntityLight;
	struct EntityModel;
}

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESCPRoomCreatorFlags : uint8
{
	None = 0 UMETA(Hidden),
	Screens = 1 << 0,
	SoundEmitters = 1 << 1,
	SpotLights = 1 << 2,
	PointLights = 1 << 3,
	Models = 1 << 4,
	RoomMesh = 1 << 5,
};
ENUM_CLASS_FLAGS(ESCPRoomCreatorFlags);

UCLASS(BlueprintType)
class SCPEDITOR_API USCPRoomCreatorData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString RoomName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MeshName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString BPOutputPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MeshOutputPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString RMeshPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESCPRoomType RoomType = ESCPRoomType::TwoWay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSCPZoneSelection RoomZone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bSpecialRoom = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tool Settings")
	bool bGenerateMesh = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tool Settings|Level")
	bool bGenerateLevel = true;

	/** What entities to import from the RMesh file. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tool Settings|Level", meta = (Bitmask, BitmaskEnum = "/Script/SCPEditor.ESCPRoomCreatorFlags", EditCondition = "bGenerateLevel && !bOverrideLevel"))
	uint8 ImportFlags = 63;

	/** Whether or not to forcefully create a new level instead of updating existing entities within it. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tool Settings|Level", meta = (EditCondition = "bGenerateLevel"))
	bool bOverrideLevel = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tool Settings")
	bool bGenerateDataAsset = true;

	bool HasImportFlag(ESCPRoomCreatorFlags Flag) const { return (ImportFlags & (uint8)Flag) == (uint8)Flag; }
};

/**
 * @class USCPRoomCreatorWidget
 */
UCLASS()
class SCPEDITOR_API USCPRoomCreatorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	TArray<FString> SelectRMeshFiles();

	UFUNCTION(BlueprintCallable)
	bool AddFilesToList(TArray<FString> FilePaths);

	UFUNCTION(BlueprintCallable)
	void ClearList();

	UFUNCTION(BlueprintCallable)
	void GenerateAllRooms();

	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityListView> ListView = nullptr;

	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityEditableTextBox> BaseBPOutputPath = nullptr;

	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityEditableTextBox> BaseMeshOutputPath = nullptr;

	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityCheckBox> DoAssetCheckCheckBox = nullptr;

	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityCheckBox> HaltOnFailCheckBox = nullptr;

	/** Whether or not to append the room name to the art paths */
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityCheckBox> AppendNameArtCheckBox = nullptr;

	/** Whether or not to append the room name to the blueprint paths */
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityCheckBox> AppendNameBPCheckBox = nullptr;

	/** Sets the default value for GenerateLevel on the list data */
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityCheckBox> DefaultGenerateLevelCheckBox = nullptr;

	/** Sets the default value for OverrideLevel on the list data */
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityCheckBox> DefaultOverrideLevelCheckBox = nullptr;

	/** Sets the default value for GenerateMesh on the list data */
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityCheckBox> DefaultGenerateMeshCheckBox = nullptr;

	/** Sets the default value for GenerateDataAsset on the list data */
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityCheckBox> DefaultGenerateDataAssetCheckBox = nullptr;

	/** Checks whether or not the asset exists before creating an asset at the same path */
	UPROPERTY(BlueprintReadWrite)
	bool bDoAssetCheck;

	/** Controls whether or not to stop importing if a step fails. */
	UPROPERTY(BlueprintReadWrite)
	bool bHaltOnFailure;

	/** Whether or not to append the room name to the art paths */
	UPROPERTY(BlueprintReadWrite)
	bool bAppendNameToArtPath;

	/** Whether or not to append the room name to the blueprint paths */
	UPROPERTY(BlueprintReadWrite)
	bool bAppendNameToBPPath;

	/** Sets the default value for GenerateLevel on the list data */
	UPROPERTY(BlueprintReadWrite)
	bool bDefaultGenerateLevel;

	/** Sets the default value for OverrideLevel on the list data */
	UPROPERTY(BlueprintReadWrite)
	bool bDefaultOverrideLevel;

	/** Sets the default value for GenerateMesh on the list data */
	UPROPERTY(BlueprintReadWrite)
	bool bDefaultGenerateMesh;

	/** Sets the default value for GenerateDataAsset on the list data */
	UPROPERTY(BlueprintReadWrite)
	bool bDefaultGenerateDataAsset;

private:
	/** Returns true on yes, false on no */
	bool ThrowYesNoMessage(FString Title, FString Body);

	bool ImportStaticMesh(TObjectPtr<USCPRoomCreatorData> Data);

	bool GenerateRoomAsset(TObjectPtr<USCPRoomCreatorData> Data);

	// Level Functions
	TObjectPtr<AActor> FindActorByName(TObjectPtr<UWorld> World, FName Name);

	bool GenerateRoomLevel(TObjectPtr<USCPRoomCreatorData> Data);

	/** Imports a fresh level if one does not exist already. */
	bool ImportFreshLevel(rmesh::RMesh& File, bool bRMeshValid, TObjectPtr<UWorld> World, TObjectPtr<USCPRoomCreatorData> Data);

	/** Updates existing entities rather than completely removing the whole map */
	bool ImportExistingLevel(rmesh::RMesh& File, bool bRMeshValid, TObjectPtr<UWorld> World, TObjectPtr<USCPRoomCreatorData> Data);

	void ImportRMeshEntities(rmesh::RMesh& File, TObjectPtr<UWorld> World);
	void ImportRMeshCollisions(rmesh::RMesh& File, TObjectPtr<UWorld> World);
	void ImportRMeshTriggers(rmesh::RMesh& File, TObjectPtr<UWorld> World);

	// RMesh find or create functions

	void UpdateScreenEntity(rmesh::EntityScreen& Screen, TObjectPtr<ANote> Actor, TObjectPtr<UWorld> World);
	void UpdateSoundEmitterEntity(rmesh::EntitySoundEmitter& SoundEmitter, TObjectPtr<ANote> Actor, TObjectPtr<UWorld> World);
	void UpdateSpotLightEntity(rmesh::EntitySpotLight& SpotLight, TObjectPtr<ASpotLight> Actor, TObjectPtr<UWorld> World);
	void UpdatePointLightEntity(rmesh::EntityLight& Light, TObjectPtr<APointLight> Actor, TObjectPtr<UWorld> World);
	void UpdateModelEntity(rmesh::EntityModel& Model, TObjectPtr<ANote> Actor, TObjectPtr<UWorld> World);

	TObjectPtr<UClass> DataAssetClass;
};
