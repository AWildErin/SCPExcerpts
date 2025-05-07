// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SCPPlayerMovementComponent.generated.h"

/**
 * @class USCPPlayerMovementComponent
 */
UCLASS()
class USCPPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:

	// Begin UMovementComponent Interface
	virtual float GetMaxSpeed() const override;
	// End UMovementComponent Interface
};
