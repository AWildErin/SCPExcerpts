#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"

#include "SCPMaterialCreatorWidget.generated.h"

class UButton;

/**
* @class USCPMaterialCreatorWidget
* A general helper to quickly create SCP:CB materials.
*/
UCLASS()
class SCPEDITOR_API USCPMaterialCreatorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateMaterialsFromSelected();

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CreateMaterialsButton = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UMaterial> BaseMaterial;
};
