#include "AssetTypeActions/SCPAssetTypeActions_ItemDataAsset.h"

#include "Data/SCPItemDataAsset.h"
#include "SCPEditorModule.h"

#define LOCTEXT_NAMESPACE "SCPEditor"

FText FSCPAssetTypeActions_ItemDataAsset::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "FSCPAssetTypeActions_ItemDataAsset", "Item Data");
}

UClass* FSCPAssetTypeActions_ItemDataAsset::GetSupportedClass() const
{
	return USCPItemDataAsset::StaticClass();
}

FColor FSCPAssetTypeActions_ItemDataAsset::GetTypeColor() const
{
	return FColor(148, 123, 104);
}

bool FSCPAssetTypeActions_ItemDataAsset::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

uint32 FSCPAssetTypeActions_ItemDataAsset::GetCategories()
{
	FSCPEditorModule& EditorModule = FModuleManager::LoadModuleChecked<FSCPEditorModule>("SCPEditor");
	return EditorModule.GetAssetTypeCategory();
}

#undef LOCTEXT_NAMESPACE
