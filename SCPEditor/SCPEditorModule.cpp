#include "SCPEditorModule.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Logging/StructuredLog.h"

#include "AssetTypeActions/SCPAssetTypeActions_ItemDataAsset.h"
#include "AssetTypeActions/SCPAssetTypeActions_RoomDataAsset.h"
#include "DetailCustomizations/SCPZoneSelectCustomization.h"
#include "Map/SCPRoomDataAsset.h"

#define LOCTEXT_NAMESPACE "SCPEditor"

DEFINE_LOG_CATEGORY(LogSCPEditor);

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

/** Called when the editor module is called */
void FSCPEditorModule::StartupModule()
{
	UE_LOG(LogSCPEditor, Log, TEXT("FSCPEditorModule: Module Started"));

	// Detail Customizations
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout(FSCPZoneSelection::StaticStruct()->GetFName(),
														FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSCPZoneSelectCustomization::MakeInstance));

		PropertyModule.NotifyCustomizationModuleChanged();
	}

	// Asset Types
	{
		IAssetTools& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTypeCategory = AssetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("SCP")), LOCTEXT("SCPCategory", "SCP"));

		TSharedPtr<FSCPAssetTypeActions_ItemDataAsset> ItemDataAssetActions = MakeShareable(new FSCPAssetTypeActions_ItemDataAsset());
		AssetToolsModule.RegisterAssetTypeActions(ItemDataAssetActions.ToSharedRef());

		TSharedPtr<FSCPAssetTypeActions_RoomDataAsset> RoomDataAssetActions = MakeShareable(new FSCPAssetTypeActions_RoomDataAsset());
		AssetToolsModule.RegisterAssetTypeActions(RoomDataAssetActions.ToSharedRef());
	}
}

/** Called when the editor module is shutdown */
void FSCPEditorModule::ShutdownModule()
{
	UE_LOG(LogSCPEditor, Log, TEXT("FSCPEditorModule: Module Ended"));

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSCPZoneSelection::StaticStruct()->GetFName());
		PropertyModule.NotifyCustomizationModuleChanged();
	}

	if (IPlacementModeModule::IsAvailable())
	{
		IPlacementModeModule::Get().UnregisterPlacementCategory("SCPEditor");
	}
}

IMPLEMENT_MODULE(FSCPEditorModule, SCPEditor);

#undef LOCTEXT_NAMESPACE
