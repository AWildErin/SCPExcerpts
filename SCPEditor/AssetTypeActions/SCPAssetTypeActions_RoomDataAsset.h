#pragma once

#include "CoreMinimal.h"

#include "AssetTypeActions_Base.h"

class FSCPAssetTypeActions_RoomDataAsset : public FAssetTypeActions_Base
{
public:
	// Begin IAssetTypeActions Interface
	FText GetName() const override;
	UClass* GetSupportedClass() const override;
	FColor GetTypeColor() const override;
	bool HasActions(const TArray<UObject*>& InObjects) const override;
	uint32 GetCategories() override;
	// End IAssetTypeActions Interface
};
