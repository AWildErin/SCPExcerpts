#pragma once

#include "CoreMinimal.h"
#include "DevUIWindow.h"

#include "SCPInventoryWindow.generated.h"

class USCPInventoryComponent;

/**
 * @class USCPInventoryWindow
 * @brief Draws various buttons to allow cheats
 */
UCLASS()
class USCPInventoryWindow : public UDevUIWindow
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
	void DrawInventoryContents(USCPInventoryComponent* Inventory);

	USCPInventoryComponent* SelectedInventory;
};
