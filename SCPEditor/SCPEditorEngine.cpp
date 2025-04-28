// Fill out your copyright notice in the Description page of Project Settings.

#include "SCPEditorEngine.h"

#include "SCPEditorModule.h"

#include "ISourceControlModule.h"
#include "ISourceControlProvider.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SCPEditorEngine)

#define LOCTEXT_NAMESPACE "SCPEditor"

USCPEditorEngine::USCPEditorEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USCPEditorEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);

	UE_LOG(LogSCPEditor, Log, TEXT("USCPEditorEngine: Init"));

	const ISourceControlModule& SourceControlModule = ISourceControlModule::Get();
	{
		/** @todo Get automatic merging working for branches*/
		ISourceControlProvider& SourceControlProvider = SourceControlModule.GetProvider();
		const TArray<FString> Branches { "origin/main" };
		SourceControlProvider.RegisterStateBranches(Branches, TEXT(""));
	}
}

void USCPEditorEngine::Start()
{
	Super::Start();

	UE_LOG(LogSCPEditor, Log, TEXT("USCPEditorEngine: Start"));
}

void USCPEditorEngine::Tick(float DeltaSeconds, bool bIdleMode)
{
	Super::Tick(DeltaSeconds, bIdleMode);
}

#undef LOCTEXT_NAMESPACE
