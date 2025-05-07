// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "SCPGameEngine.generated.h"

/**
 * @class USCPGameEngine
 */
UCLASS()
class USCPGameEngine : public UGameEngine
{
	GENERATED_BODY()

public:
	USCPGameEngine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	// Begin UEngine interface
	virtual void Init(class IEngineLoop* InEngineLoop) override;
	virtual void Start() override;
	virtual void Tick(float DeltaSeconds, bool bIdleMode) override;
	// End UEngine interface
};
