// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// clang-format off
DECLARE_LOG_CATEGORY_EXTERN(LogSCP,				Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPPlayer,		Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPInput,		Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPAI,			Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPInteraction,	Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPDamage,		Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPMap,			Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPItem,			Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPCheat,		Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPUI,			Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSCPDev,			Log, All);
// clang-format on

// Various config variables used in SCP

// In SCP:CB a tick is 1/70th of a second, we use this value here to make sure various multiplications are correct
// Such as in the blinking code where we do (DeltaTime * SCP_BLITZ_TICK) * 0.6f * BlinkDrainRate
#define SCP_BLITZ_TICK 70.f

// #ifndef SCP_USE_STEAM
//	#define SCP_USE_STEAM 1
// #endif

/**
 * @class FSCPGameModule
 */
class FSCPGameModule : public FDefaultGameModuleImpl
{
public:
	/** Called when the game module is shutdown */
	virtual void StartupModule() override;

	/** Called when the game module is shutdown */
	virtual void ShutdownModule() override;
};
