#include "Windows/Misc/SCPCheatWindow.h"
#include "DevUIWindow.h"

#include "Player/SCPPlayerController.h"
#include "Player/SCPCheatManager.h"

#include "BYGImguiLoggerSubsystem.h"

bool USCPCheatWindow::Draw(UWorld* World)
{
	ImGui::Separator();
	DrawViewModeButtons(World);

	ImGui::Separator();
	DrawTeleportEntry(World);

	ImGui::Separator();
	if (ImGui::Button(TCHAR_TO_ANSI(TEXT("Toggle Infinite Stamina"))))
	{
		auto PlayerController = World->GetFirstPlayerController<ASCPPlayerController>();
		auto CheatManager = Cast<USCPCheatManager>(PlayerController->CheatManager);

		if (CheatManager != nullptr)
		{
			CheatManager->InfiniteStamina();
		}
	}

	return true;
}

FString USCPCheatWindow::GetWindowName()
{
	return TEXT("Cheats");
}

FString USCPCheatWindow::GetWindowHelp()
{
	return TEXT("");
}

ImGuiWindowFlags USCPCheatWindow::GetWindowFlags() const
{
	// return ImGuiWindowFlags_MenuBar;
	return ImGuiWindowFlags_None;
}

void USCPCheatWindow::DrawViewModeButtons(UWorld* World)
{
	ImGui::BeginGroup();
	ImGui::Text(TCHAR_TO_ANSI(TEXT("View modes")));

	if (ImGui::Button(TCHAR_TO_ANSI(TEXT("Lit"))))
	{
		World->GetGameViewport()->ViewModeIndex = EViewModeIndex::VMI_Lit;
	}
	ImGui::SameLine();

	if (ImGui::Button(TCHAR_TO_ANSI(TEXT("Unlit"))))
	{
		World->GetGameViewport()->ViewModeIndex = EViewModeIndex::VMI_Unlit;
	}
	ImGui::SameLine();

	if (ImGui::Button(TCHAR_TO_ANSI(TEXT("Wireframe"))))
	{
		World->GetGameViewport()->ViewModeIndex = EViewModeIndex::VMI_Wireframe;
	}

	ImGui::EndGroup();
}

void USCPCheatWindow::DrawTeleportEntry(UWorld* World)
{
	ImGui::BeginGroup();

	static char Buffer[256];
	ImGui::InputText("Teleport to room", Buffer, IM_ARRAYSIZE(Buffer));

	if (ImGui::Button(TCHAR_TO_ANSI(TEXT("Submit"))))
	{
		auto PlayerController = World->GetFirstPlayerController<ASCPPlayerController>();
		auto CheatManager = Cast<USCPCheatManager>(PlayerController->CheatManager);

		if (CheatManager != nullptr)
		{
			CheatManager->TeleportToRoom(FString(UTF8_TO_TCHAR(Buffer)));
		}
	}

	ImGui::EndGroup();
}
