// Fill out your copyright notice in the Description page of Project Settings.

#include "SCPGameEngine.h"
#include "SCPGameModule.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SCPGameEngine)

USCPGameEngine::USCPGameEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USCPGameEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);

	UE_LOG(LogSCP, Log, TEXT("USCPGameEngine: Init"));
}

void USCPGameEngine::Start()
{
	Super::Start();

	UE_LOG(LogSCP, Log, TEXT("USCPGameEngine: Start"));
}

void USCPGameEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	Super::Tick(DeltaSeconds, bIdleMode);
}

#undef LOCTEXT_NAMESPACE
