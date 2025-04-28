#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interaction/SCPInteractInterface.h"

#include "SCPBaseButton.generated.h"

class UArrowComponent;
class UFMODAudioComponent;
class UFMODEvent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSCPButtonEvent, AActor*, Interactor);

UENUM(BlueprintType)
enum class ESCPButtonType : uint8
{
	None,
	Push,
	KeyCard,
	KeyPad,
	Scanner
};

/**
* @class ASCPBaseButton
*
* @todo Add code and card level to the class so we can properly handle them. The need to be here as this actor is spawned in C++
* and as such we can't add them to the blueprint classes for obvious reasons.
*/
UCLASS()
class ASCPBaseButton
	: public AActor
	, public ISCPInteractInterface
{
	GENERATED_BODY()

public:
	ASCPBaseButton();

	// Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// End AActor Interface

	// Begin ISCPInteractInterface
	bool RequiresFocus_Implementation() override { return false; }
	bool CanInteract_Implementation(AActor* Interactor) override { return true; }
	void OnInteract_Implementation(AActor* Interactor) override;
	// End ISCPInteractInterface

	/** Blueprint implementable event to be able to have control over the button process. Here you can implement things like checking the keypad code, or the keycard type. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Button")
	bool BeginPress(AActor* Interactor);

	/** Finishes up the button press. The delegate is fired here*/
	UFUNCTION(BlueprintCallable, Category = "Button")
	void FinishPress(AActor* Interactor);

	/** Called when the player first begins to interact with the button */
	UPROPERTY(BlueprintAssignable)
	FSCPButtonEvent OnButtonPressBegin;

	/** Called if BeginPress returns true */
	UPROPERTY(BlueprintAssignable)
	FSCPButtonEvent OnButtonPressSuccess;

	/** Called if BeginPress returns false */
	UPROPERTY(BlueprintAssignable)
	FSCPButtonEvent OnButtonPressFailed;

	UPROPERTY(EditDefaultsOnly, Category = "Button")
	TObjectPtr<UStaticMeshComponent> ButtonMesh;

	/** @custom_region{Audio} */

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Button|Audio")
	TObjectPtr<UFMODAudioComponent> ButtonAudioComponent;

	UPROPERTY(EditAnywhere, Category = "Button|Audio")
	bool bMuteSounds;

	UPROPERTY(EditAnywhere, Category = "Button|Audio")
	TObjectPtr<UFMODEvent> PressedSoundEvent;

	UPROPERTY(EditAnywhere, Category = "Button|Audio")
	TObjectPtr<UFMODEvent> SuccessSoundEvent;

	UPROPERTY(EditAnywhere, Category = "Button|Audio")
	TObjectPtr<UFMODEvent> FailedSoundEvent;

	/** @} */

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;
#endif

	/** Whether or not the butten is currently being interacted with */
	bool bPressInProgress;
};
