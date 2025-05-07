// Fill out your copyright notice in the Description page of Project Settings.

#include "SCPPlayerMovementComponent.h"
#include "SCPPlayerCharacter.h"

float USCPPlayerMovementComponent::GetMaxSpeed() const
{
	if (ASCPPlayerCharacter* Owner = Cast<ASCPPlayerCharacter>(PawnOwner))
	{
		// TODO: Add some checks here to see if we're *actually* allowed to sprint
		// Otherwise we rely on the drain code to stop us from sprinting
		if (Owner->bIsSprinting && !IsCrouching())
		{
			return Owner->SprintSpeed;
		}

		if (IsCrouching())
		{
			return Owner->CrouchSpeed;
		}

		return Owner->WalkSpeed;
	}

	return Super::GetMaxSpeed();
}
