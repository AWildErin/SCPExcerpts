#include "Player/SCPPlayerController.h"

#include "Player/SCPCheatManager.h"

ASCPPlayerController::ASCPPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CheatClass = USCPCheatManager::StaticClass();
}
