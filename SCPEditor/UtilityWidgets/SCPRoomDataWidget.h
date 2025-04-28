#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"

#include "SCPRoomDataWidget.generated.h"

class UEditorUtilityEditableTextBox;
class USCPRoomDataAsset;

/**
 * @class USCPRoomDataWidget
 */
UCLASS(Blueprintable)
class SCPEDITOR_API USCPRoomDataWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void GatherDataAssets();

	UFUNCTION(BlueprintCallable)
	void AssignRoomIniValues();

	UFUNCTION(BlueprintCallable)
	void GenerateRoomExtents();

	UFUNCTION(BlueprintCallable)
	FString SelectIniFile();

	UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
	TObjectPtr<UEditorUtilityEditableTextBox> RoomIniPath = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TMap<USCPRoomDataAsset*, FAssetData> RoomAssets;
};
