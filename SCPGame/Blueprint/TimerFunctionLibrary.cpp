// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/TimerFunctionLibrary.h"

float UTimerFunctionLibrary::GetElapsedPercentage(const UObject* WorldContext, FTimerHandle Timer, float Duration)
{
	FTimerManager& TimerManager = WorldContext->GetWorld()->GetTimerManager();

	return TimerManager.GetTimerElapsed(Timer) / Duration;
}

float UTimerFunctionLibrary::GetRemainingPercentage(const UObject* WorldContext, FTimerHandle Timer, float Duration)
{
	FTimerManager& TimerManager = WorldContext->GetWorld()->GetTimerManager();

	return TimerManager.GetTimerRemaining(Timer) / Duration;
}

float UTimerFunctionLibrary::GetDuration(const UObject* WorldContext, FTimerHandle Timer)
{
	FTimerManager& TimerManager = WorldContext->GetWorld()->GetTimerManager();

	float ElapsedTime = TimerManager.GetTimerElapsed(Timer);
	float RemainingTime = TimerManager.GetTimerRemaining(Timer);

	return RemainingTime + ElapsedTime;
}
