#include "AssetTypeActions/SCPAssetTypeActions_RoomDataAsset.h"

#include "Map/SCPRoomDataAsset.h"
#include "SCPEditorModule.h"

#define LOCTEXT_NAMESPACE "SCPEditor"

FText FSCPAssetTypeActions_RoomDataAsset::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "FSCPAssetTypeActions_RoomDataAsset", "Room Data");
}

UClass* FSCPAssetTypeActions_RoomDataAsset::GetSupportedClass() const
{
	return USCPRoomDataAsset::StaticClass();
}

FColor FSCPAssetTypeActions_RoomDataAsset::GetTypeColor() const
{
	return FColor(148, 123, 104);
}

bool FSCPAssetTypeActions_RoomDataAsset::HasActions(const TArray<UObject*>& InObjects) const
{
	return false;
}

uint32 FSCPAssetTypeActions_RoomDataAsset::GetCategories()
{
	FSCPEditorModule& EditorModule = FModuleManager::LoadModuleChecked<FSCPEditorModule>("SCPEditor");
	return EditorModule.GetAssetTypeCategory();
}

#undef LOCTEXT_NAMESPACE
