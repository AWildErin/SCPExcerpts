#pragma once

#include "Engine/DataAsset.h"
#include "SCPItemDataAsset.generated.h"

/**
* @class USCPItemDataAsset
* Data asset holding basic information about an item, such as icon, name, description
*/
UCLASS(BlueprintType)
class SCPGAME_API USCPItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ItemName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ItemDescription;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName CheatName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSoftObjectPtr<UTexture2D> ItemIcon;

	/** Specifies whether or not we show an icon in the middle of the screen when equiped */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipping")
	bool bUseEquipIcon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipping", meta = (EditCondition = "bUseEquipIcon"))
	TSoftObjectPtr<UTexture2D> EquipIcon;

	/** Whether or not to show a timer when equiping */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipping")
	bool bUseEquipTimer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipping", meta = (EditCondition = "bUseEquipIcon"))
	float EquipTimer;
};
