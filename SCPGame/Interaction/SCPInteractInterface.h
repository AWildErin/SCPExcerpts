// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SCPInteractInterface.generated.h"

UINTERFACE(BlueprintType)
class USCPInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ISCPInteractInterface
 * Interaction interface to determine whether or not an actor is an interactable
 * Use ASCPInteractActor instead of the interface directly
 */
class ISCPInteractInterface
{
	GENERATED_BODY()
public:

	/** Called when the interact button is pressed */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* Interactor);

	/** Called when the interact button is held down */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void BeginGrabbing(AActor* Interactor);

	/** Called when the interact button is released */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndGrabbing(AActor* Interactor);

	/** Returns whether or not we can interacted with */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(AActor* Interactor);

	/** @returns whether or not we require focus */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool RequiresFocus();
};
