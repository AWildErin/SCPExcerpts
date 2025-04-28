#pragma once

#include "CoreMinimal.h"
#include "DevUIWindow.h"
#include "SCPCheatWindow.generated.h"

/**
 * @class USCPCheatWindow
 * @brief Draws various buttons to allow cheats
 */
UCLASS()
class USCPCheatWindow : public UDevUIWindow
{
	GENERATED_BODY()
public:
	/**
	 * Draws the window if enabled
	 * @return True to indicate this window should remain open
	 */
	bool Draw(UWorld* World) override;

	/** Returns FString that will be used for the title */
	FString GetWindowName() override;

	/** Returns the window help text */
	FString GetWindowHelp() override;

	/** Returns ImGui windows flags */
	ImGuiWindowFlags GetWindowFlags() const override;

private:
	void DrawViewModeButtons(UWorld* World);
	void DrawTeleportEntry(UWorld* World);

	bool bShowLogWindow = false;
};
