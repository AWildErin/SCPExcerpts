#include "UtilityWidgets/SCPMaterialCreatorWidget.h"

#include "Components/Button.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "AssetToolsModule.h"
#include "Subsystems/EditorAssetSubsystem.h"
#include "EditorUtilityLibrary.h"

#include "SCPEditorModule.h"

#define LogDebug(format, ...) UE_LOG(LogSCPEditor, Log, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogWarning(format, ...) UE_LOG(LogSCPEditor, Warning, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogError(format, ...) UE_LOG(LogSCPEditor, Error, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)

void USCPMaterialCreatorWidget::CreateMaterialsFromSelected()
{
	if (!BaseMaterial)
	{
		LogError(TEXT("BaseMaterial was null!"));
	}

	FName NAME_DiffuseTexture = FName("Diffuse Texture");
	FName NAME_NormalTexture = FName("Normal Texture");

	FName NAME_MetallicSwitch = FName("Use Metallic Texture");
	FName NAME_MetallicTexture = FName("Metallic Texture");

	FName NAME_RoughnessSwitch = FName("Use Roughness Texture");
	FName NAME_RoughnessTexture = FName("Roughness Texture");

	FName NAME_SpecularSwitch = FName("Use Specular Texture");
	FName NAME_SpecularTexture = FName("Specular Texture");

	TArray<UObject*> Assets = UEditorUtilityLibrary::GetSelectedAssetsOfClass(UTexture::StaticClass());
	UE_LOG(LogSCPEditor, Warning, TEXT("Test"));

	const FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& AssetTools = AssetToolsModule.Get();
	TObjectPtr<UMaterialInstanceConstantFactoryNew> Factory = NewObject<UMaterialInstanceConstantFactoryNew>();

	TObjectPtr<UEditorAssetSubsystem> AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();

	for (auto& TextureObject : Assets)
	{
		TObjectPtr<UTexture> Texture = Cast<UTexture>(TextureObject);

		LogWarning(TEXT("%s"), *Texture->GetPathName());

		FString DiffuseTextureName = Texture->GetName();
		if (!DiffuseTextureName.StartsWith("T_") || !DiffuseTextureName.EndsWith("_D"))
		{
			LogError(TEXT("%s did not follow naming convention!"), *DiffuseTextureName);
			continue;
		}

		FString NormalTextureName = DiffuseTextureName.LeftChop(1) + "N";
		FString MetallicTextureName = DiffuseTextureName.LeftChop(1) + "M";
		FString RoughnessTextureName = DiffuseTextureName.LeftChop(1) + "R";
		FString SpecularTextureName = DiffuseTextureName.LeftChop(1) + "S";

		// Texture name with no prefix or suffix
		FString BaseTextureName = DiffuseTextureName.LeftChop(2).RightChop(2);
		FString MaterialName = "MI_" + BaseTextureName;
		FString Directory = FPaths::GetPath(Texture->GetPathName());

		if (AssetSubsystem->DoesAssetExist(Directory / MaterialName))
		{
			LogWarning(TEXT("%s/%s already exists. Skipping"), *Directory, *MaterialName);
			continue;
		}

		TObjectPtr<UMaterialInstanceConstant> MaterialInstance =
			Cast<UMaterialInstanceConstant>(AssetTools.CreateAsset(MaterialName, Directory, UMaterialInstanceConstant::StaticClass(), Factory));

		MaterialInstance->Parent = BaseMaterial;
		MaterialInstance->SetTextureParameterValueEditorOnly(FMaterialParameterInfo(NAME_DiffuseTexture), Texture);

		if (AssetSubsystem->DoesAssetExist(Directory / NormalTextureName))
		{
			TObjectPtr<UTexture> NormTexture = Cast<UTexture>(AssetSubsystem->LoadAsset(Directory / NormalTextureName));

			if (NormTexture)
			{
				MaterialInstance->SetTextureParameterValueEditorOnly(FMaterialParameterInfo(NAME_NormalTexture), NormTexture);
			}
			else
			{
				LogError(TEXT("Failed to find normal texture for %s!"), *DiffuseTextureName);
			}
		}

		if (AssetSubsystem->DoesAssetExist(Directory / MetallicTextureName))
		{
			TObjectPtr<UTexture> MetalTexture = Cast<UTexture>(AssetSubsystem->LoadAsset(Directory / MetallicTextureName));

			if (MetalTexture)
			{
				MaterialInstance->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(NAME_MetallicSwitch), true);
				MaterialInstance->SetTextureParameterValueEditorOnly(FMaterialParameterInfo(NAME_MetallicTexture), MetalTexture);
			}
			else
			{
				LogError(TEXT("Failed to find metallic texture for %s!"), *DiffuseTextureName);
			}
		}

		if (AssetSubsystem->DoesAssetExist(Directory / RoughnessTextureName))
		{
			TObjectPtr<UTexture> MetalTexture = Cast<UTexture>(AssetSubsystem->LoadAsset(Directory / RoughnessTextureName));

			if (MetalTexture)
			{
				MaterialInstance->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(NAME_RoughnessSwitch), true);
				MaterialInstance->SetTextureParameterValueEditorOnly(FMaterialParameterInfo(NAME_RoughnessTexture), MetalTexture);
			}
			else
			{
				LogError(TEXT("Failed to find metallic texture for %s!"), *DiffuseTextureName);
			}
		}

		if (AssetSubsystem->DoesAssetExist(Directory / SpecularTextureName))
		{
			TObjectPtr<UTexture> MetalTexture = Cast<UTexture>(AssetSubsystem->LoadAsset(Directory / SpecularTextureName));

			if (MetalTexture)
			{
				MaterialInstance->SetStaticSwitchParameterValueEditorOnly(FMaterialParameterInfo(NAME_SpecularSwitch), true);
				MaterialInstance->SetTextureParameterValueEditorOnly(FMaterialParameterInfo(NAME_SpecularTexture), MetalTexture);
			}
			else
			{
				LogError(TEXT("Failed to find metallic texture for %s!"), *DiffuseTextureName);
			}
		}
	}
}
