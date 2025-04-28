#include "UtilityWidgets/SCPRMeshMaterialWidget.h"

#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"

#include "AssetToolsModule.h"
#include "Subsystems/EditorAssetSubsystem.h"
#include "EditorUtilityLibrary.h"

#include "SCPEditorSettings.h"

#include "SCPEditorModule.h"

#define LogDebug(format, ...) UE_LOG(LogSCPEditor, Log, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogWarning(format, ...) UE_LOG(LogSCPEditor, Warning, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogError(format, ...) UE_LOG(LogSCPEditor, Error, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)

void USCPRMeshMaterialWidget::ApplyMaterialsToSelected()
{
	TObjectPtr<UEditorAssetSubsystem> AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	TArray<UObject*> Assets = UEditorUtilityLibrary::GetSelectedAssetsOfClass(UStaticMesh::StaticClass());

	const USCPEditorSettings* EditorSettings = USCPEditorSettings::Get();

	for (auto& ModelObject : Assets)
	{
		TObjectPtr<UStaticMesh> Model = Cast<UStaticMesh>(ModelObject);
		TArray<FStaticMaterial>& Materials = Model->GetStaticMaterials();

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
				LogError(TEXT("%s did not exist on the material map!"), *Material.MaterialSlotName.ToString());
			}
		}

		Model->SetStaticMaterials(Materials);
		if (!AssetSubsystem->SaveAsset(Model->GetPackage()->GetPathName(), false))
		{
			LogError("Failed to save %s", *Model->GetPackage()->GetPathName());
		}
	}
}
