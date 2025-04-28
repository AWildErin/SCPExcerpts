#include "Windows/Player/SCPPlayerWindow.h"
#include "DevUIWindow.h"

#include "Items/SCPBaseItem.h"
#include "Player/SCPPlayerCharacter.h"
#include "Player/SCPPlayerMovementComponent.h"
#include "Components/SCPBlinkComponent.h"
#include "Components/SCPInventoryComponent.h"

bool USCPPlayerWindow::Draw(UWorld* World)
{
	ASCPPlayerCharacter* Pawn = GetCurrentPlayer(World);
	if (!Pawn)
	{
		ImGui::Text("No compatible pawn!");
		return true;
	}

	if (ImGui::BeginTabBar("PlayerCharacterTabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("General"))
		{
			DrawGeneralTab(Pawn, World);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Inventory"))
		{
			DrawInventoryTab(Pawn, World);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	return true;
}

FString USCPPlayerWindow::GetWindowName()
{
	return TEXT("Pawn Debug");
}

FString USCPPlayerWindow::GetWindowHelp()
{
	return TEXT("Displays various debugging info about the current pawn");
}

ImGuiWindowFlags USCPPlayerWindow::GetWindowFlags() const
{
	// return ImGuiWindowFlags_MenuBar;
	return ImGuiWindowFlags_None;
}

ASCPPlayerCharacter* USCPPlayerWindow::GetCurrentPlayer(UWorld* World)
{
	APlayerController* PC = World->GetFirstPlayerController();

	if (!PC || !PC->GetPawn())
	{
		return nullptr;
	}

	return Cast<ASCPPlayerCharacter>(PC->GetPawn());
}

void USCPPlayerWindow::DrawGeneralTab(ASCPPlayerCharacter* Player, UWorld* World)
{
	USCPBlinkComponent* Blink = Player->BlinkComponent;

	// Row 1
	ImGui::BeginGroup();
	ImGui::Text("Blink Frequency: %.2f", Blink->GetBlinkFrequency());
	ImGui::Text("Blink Timer: %.2f", Blink->GetBlinkTimer());

	FString IsBlinking = Blink->GetIsBlinking() ? "True" : "False";
	ImGui::Text("Is Blinking: %s", TCHAR_TO_ANSI(*IsBlinking));

	ImGui::Text("Stamina: %.2f/%.2f", Player->StaminaCurrentValue, Player->StaminaMaxValue);
	ImGui::EndGroup();

	ImGui::SameLine();
	ImGui::BeginGroup();
	FString ClosestInteractableName = Player->ClosestInteractable ? Player->ClosestInteractable->GetName() : "";
	FString FocusedInteractableName = Player->FocusedInteractable ? Player->FocusedInteractable->GetName() : "";

	ImGui::Text("Closest Interactable: %s", TCHAR_TO_ANSI(*ClosestInteractableName));
	ImGui::Text("Focused Interactable: %s", TCHAR_TO_ANSI(*FocusedInteractableName));
	ImGui::EndGroup();
}

void USCPPlayerWindow::DrawInventoryTab(ASCPPlayerCharacter* Player, UWorld* World)
{
	USCPInventoryComponent* Inventory = Player->InventoryComponent;
	auto Array = Inventory->GetInventoryArray();

	if (ImGui::BeginTable("InventoryTable", 3))
	{
		for (int Row = 0; Row < Array.Num(); Row++)
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%d", Row);

			auto Item = Array[Row];
			ImGui::TableSetColumnIndex(1);
			if (Item)
			{
				ImGui::Text("%s", TCHAR_TO_ANSI(*Item->GetName()));
			}
			else
			{
				ImGui::Text("None");
			}

			ImGui::TableSetColumnIndex(2);
			if (Item)
			{
				if (ImGui::Button("Drop Item"))
				{
					Inventory->DropItem(Item);
				}
			}
		}
		ImGui::EndTable();
	}
}
