#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"

#include "SCPRMeshMaterialWidget.generated.h"

/**
 * @class USCPRMeshMaterialWidget
 * A tool that applies all materials to the selected RMesh file(s)
 */
UCLASS(Config = "Editor", DefaultConfig)
class SCPEDITOR_API USCPRMeshMaterialWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ApplyMaterialsToSelected();
};
