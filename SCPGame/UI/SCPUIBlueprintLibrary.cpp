#include "UI/SCPUIBlueprintLibrary.h"

#include "Widgets/CommonActivatableWidgetContainer.h"

#include "SCPGameModule.h"

USCPActivatableWidget* USCPUIBlueprintLibrary::PushWidget(UCommonActivatableWidgetContainerBase* StackRef, TSubclassOf<USCPActivatableWidget> ActivatableWidgetClass)
{
	if (!StackRef)
	{
		UE_LOG(LogSCPUI, Error, TEXT("Invalid stack ref passed!"));
		return nullptr;
	}

	USCPActivatableWidget* Widget = CastChecked<USCPActivatableWidget>(StackRef->AddWidget(ActivatableWidgetClass));
	Widget->StackRef = StackRef;

	return Widget;
}
