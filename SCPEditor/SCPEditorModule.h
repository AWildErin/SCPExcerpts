#pragma once

#include "Logging/LogMacros.h"

#include "AssetTypeCategories.h"
#include "IPlacementModeModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSCPEditor, All, All)

/**
 * @class FSCPEditorModule
 */
class FSCPEditorModule : public FDefaultGameModuleImpl
{
public:

	// Begin IModuleInterface Interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface Interface

	FORCEINLINE EAssetTypeCategories::Type GetAssetTypeCategory() const { return AssetTypeCategory; }

private:
	EAssetTypeCategories::Type AssetTypeCategory;
	TSharedPtr<FSlateStyleSet> StyleSet;
};
