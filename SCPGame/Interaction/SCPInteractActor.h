

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SCPInteractInterface.h"

#include "SCPInteractActor.generated.h"

/**
 * @class ASCPInteractActor
 * @brief Default actor that implements ISCPInteractInterface
 *
 * This actor should be used rather than using the interface directly. 
 */
UCLASS()
class ASCPInteractActor
	: public AActor
	, public ISCPInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASCPInteractActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Whether or not this actor needs to be in focus to interact */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	bool bRequiresFocus;

	/* Interface implementations */
	bool RequiresFocus_Implementation() override { return bRequiresFocus; }
	bool CanInteract_Implementation(AActor* Interactor) override { return true; }
};
