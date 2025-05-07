#include "UI/SCPActivatableWidget.h"

TOptional<FUIInputConfig> USCPActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
		case ESCPWidgetInputMode::GameAndMenu:
			return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
		case ESCPWidgetInputMode::Game:
			return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
		case ESCPWidgetInputMode::Menu:
			return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
		case ESCPWidgetInputMode::Default:
		default:
			return TOptional<FUIInputConfig>();
	}
}
