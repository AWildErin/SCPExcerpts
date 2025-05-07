// Copyright Epic Games, Inc. All Rights Reserved.

#include "SCPGameModule.h"

#include "Engine/Console.h"
#include "Modules/ModuleManager.h"

#include "Player/SCPCheatManager.h"

DEFINE_LOG_CATEGORY(LogSCP);
DEFINE_LOG_CATEGORY(LogSCPPlayer);
DEFINE_LOG_CATEGORY(LogSCPInput);
DEFINE_LOG_CATEGORY(LogSCPAI);
DEFINE_LOG_CATEGORY(LogSCPInteraction);
DEFINE_LOG_CATEGORY(LogSCPDamage);
DEFINE_LOG_CATEGORY(LogSCPMap);
DEFINE_LOG_CATEGORY(LogSCPItem);
DEFINE_LOG_CATEGORY(LogSCPCheat);
DEFINE_LOG_CATEGORY(LogSCPUI);
DEFINE_LOG_CATEGORY(LogSCPDev);

void FSCPGameModule::StartupModule()
{
	// On public builds we can do this once, however on editor/development builds we might need to call this multiple times.
	// To solve this, we do the inverse inside USCPGameInstance.
#if UE_BUILD_SHIPPING || UE_BUILD_TEST
	UConsole::RegisterConsoleAutoCompleteEntries.AddStatic(&USCPCheatManager::AddConsoleAutoCompletes);
#endif

	UE_LOG(LogSCP, Log, TEXT("FSCPGameModule: Module Started"));
}

void FSCPGameModule::ShutdownModule()
{
	UE_LOG(LogSCP, Log, TEXT("FSCPGameModule: Module Ended"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FSCPGameModule, SCPGame, "SCPGame");
