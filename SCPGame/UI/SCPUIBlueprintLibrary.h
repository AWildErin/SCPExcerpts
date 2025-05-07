#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "UI/SCPActivatableWidget.h"

#include "SCPUIBlueprintLibrary.generated.h"

class UCommonActivatableWidgetContainerBase;

UCLASS()
class SCPGAME_API USCPUIBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Mirror of Common UI's PushWidget node, with the exception of setting StackRef which is a reference to the stack which this widget will belong to.
	* This is a placeholder until #17 is completed.
	*/
	UFUNCTION(BlueprintCallable, Category = "SCP|UI", meta = (DeterminesOutputType = ActivatableWidgetClass, DisplayName = "Push Widget"))
	static USCPActivatableWidget* PushWidget(UCommonActivatableWidgetContainerBase* StackRef, TSubclassOf<USCPActivatableWidget> ActivatableWidgetClass);
};
