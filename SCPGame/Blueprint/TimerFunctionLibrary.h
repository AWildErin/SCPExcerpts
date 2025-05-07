// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TimerFunctionLibrary.generated.h"

/**
 * @class UTimerFunctionLibrary
 * Blueprint utility library for common Timer functions
 */
UCLASS()
class UTimerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category = "Utilities|Time")
	static float GetElapsedPercentage(const UObject* WorldContext, FTimerHandle Timer, float Duration);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category = "Utilities|Time")
	static float GetRemainingPercentage(const UObject* WorldContext, FTimerHandle Timer, float Duration);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category = "Utilities|Time")
	static float GetDuration(const UObject* WorldContext, FTimerHandle Timer);
};
