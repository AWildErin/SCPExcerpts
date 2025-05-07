#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SCPActivatableWidget.generated.h"

class UCommonActivatableWidgetContainerBase;

UENUM(BlueprintType)
enum class ESCPWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

/**
 * @class USCPActivatableWidget
 * @brief An activatable widget that automatically drives the desired input config when activated
 *
 * Copied from LyraGame
 */
UCLASS(Abstract, Blueprintable)
class SCPGAME_API USCPActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:

	// Begin UCommonActivatableWidget Interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	// End UCommonActivatableWidget Interface

	/**
	* The stack that owns this widget.
	* @todo We should mirror Lyra's way on managing this.
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCommonActivatableWidgetContainerBase> StackRef;

protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ESCPWidgetInputMode InputConfig = ESCPWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
