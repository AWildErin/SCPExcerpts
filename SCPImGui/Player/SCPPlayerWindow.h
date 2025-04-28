#pragma once

#include "CoreMinimal.h"
#include "DevUIWindow.h"
#include "SCPPlayerWindow.generated.h"

class ASCPPlayerCharacter;

/**
 * @class USCPPlayerWindow
 * @brief Shows information about the current player
 */
UCLASS()
class USCPPlayerWindow : public UDevUIWindow
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
	ASCPPlayerCharacter* GetCurrentPlayer(UWorld* World);

	void DrawGeneralTab(ASCPPlayerCharacter* Player, UWorld* World);
	void DrawInventoryTab(ASCPPlayerCharacter* Player, UWorld* World);
};
