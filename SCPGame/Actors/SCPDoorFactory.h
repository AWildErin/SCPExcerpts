#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Required for the button enum
#include "SCPBaseButton.h"

#include "SCPDoorFactory.generated.h"

class ASCPBaseDoor;
class UChildActorComponent;

/**
 * @class ASCPDoorFactory
 * An actor that creates a door and 2 buttons from the specified door class and button type
 *
 * @todo Add keycard and keycode
 */
UCLASS()
class ASCPDoorFactory : public AActor
{
	GENERATED_BODY()

public:
	ASCPDoorFactory();

	// Begin AActor Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// End AActor Interface

	/** Called when we successfully interact with the button and opens the door */
	UFUNCTION()
	void OnButtonSuccess(AActor* Interactor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Data", Meta = (ExposeOnSpawn = true))
	TSubclassOf<ASCPBaseDoor> DoorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Spawn Properties", Meta = (ExposeOnSpawn = true))
	bool bStartOpen = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Door|Components")
	TObjectPtr<UChildActorComponent> DoorComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Data")
	TMap<ESCPButtonType, TSubclassOf<ASCPBaseButton>> ButtonTypeMap;

	/** The button type to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Spawn Properties", Meta = (ExposeOnSpawn = true))
	ESCPButtonType SpawnButtonType;

	// Left Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Left Button", Meta = (ExposeOnSpawn = true))
	bool bCreateLeftButton = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Door|Components")
	TObjectPtr<UChildActorComponent> LeftButtonComponent;

	// Right Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Right Button", Meta = (ExposeOnSpawn = true))
	bool bCreateRightButton = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Door|Components")
	TObjectPtr<UChildActorComponent> RightButtonComponent;
};
