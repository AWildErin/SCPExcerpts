#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "SCPBaseDoor.generated.h"

struct FTimeline;
class UArrowComponent;
class UFMODAudioComponent;
class UFMODEvent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSCPDoorEvent, ASCPBaseDoor*, DoorActor);

/**
* @class ASCPBaseDoor
* Base class for all doors in SCP:CB
*
* @todo Make a door state enum that is used to handle things like sounds and in replacement for the open/close functions.
*		We need Opening, Open, Closing, Closed. We need to also think about force opening and force closing too
*/
UCLASS()
class ASCPBaseDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASCPBaseDoor();

	// Begin AActor Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// End AActor Interface

	/**
	 * Opens the door
	 *
	 * @param bFromStart Whether or not to play the timeline from the start
	 * @param bMuteSound Whether or not to mute the door opening sound
	 */
	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor(bool bFromStart = true, bool bMuteSound = false);

	/**
	 * Closes the door
	 *
	 * @param bFromEnd Whether or not to play the timeline from the end
	 * @param bMuteSound Whether or not to mute the door closing sound
	 */
	UFUNCTION(BlueprintCallable, Category = "Door")
	void CloseDoor(bool bFromEnd = true, bool bMuteSound = false);

	/**
	 * Toggles the door
	 *
	 * @param bFromStartOrEnd Whether or not to play the timeline from the start or end
	 * @param bMuteSound Whether or not to mute the door closing sound
	 */
	UFUNCTION(BlueprintCallable, Category = "Door")
	void ToggleDoor(bool bFromStartOrEnd = true, bool bMuteSound = false);

	UPROPERTY(EditDefaultsOnly, Category = "Door|Models")
	TObjectPtr<UStaticMeshComponent> DoorFrameMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Models")
	TObjectPtr<UStaticMeshComponent> Door1Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Positions")
	TObjectPtr<USceneComponent> Door1OpenPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Models")
	TObjectPtr<UStaticMeshComponent> Door2Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Positions")
	TObjectPtr<USceneComponent> Door2OpenPosition;

	/** Specifies whether or not to show the frame */
	UPROPERTY(EditAnywhere, Category = "Door|Settings")
	bool bHideFrame;

	/** Controls whether or not this door spawns open */
	UPROPERTY(EditAnywhere, Category = "Door|Settings")
	bool bSpawnsOpen;

	UPROPERTY(BlueprintReadOnly, Category = "Door|Settings")
	bool bIsOpen;

	/** Specified whether or not the door is currently opening or closing */
	UPROPERTY(BlueprintReadOnly, Category = "Door|Settings")
	bool bIsMoving;

	/** Called when the door is beginning to Open */
	UPROPERTY(BlueprintAssignable)
	FSCPDoorEvent OnDoorOpening;

	/** Called when the door is beginning to close */
	UPROPERTY(BlueprintAssignable)
	FSCPDoorEvent OnDoorClosing;

	/** Called when the door is fully opened to closed */
	UPROPERTY(BlueprintAssignable)
	FSCPDoorEvent OnDoorCompleted;

	/** @custom_region{Audio} */

	UPROPERTY(EditDefaultsOnly, Category = "Door|Audio")
	TObjectPtr<UFMODEvent> OpenSoundEvent;

	UPROPERTY(EditDefaultsOnly, Category = "Door|Audio")
	TObjectPtr<UFMODEvent> CloseSoundEvent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Door|Audio")
	TObjectPtr<UFMODAudioComponent> DoorSoundComponent;

	/** @} */

	/** @custom_region{Timeline} */

	/** Curve to use when lerping the door */
	UPROPERTY(EditDefaultsOnly, Category = "Door|Timeline")
	TObjectPtr<UCurveFloat> DoorOpenCurve;

	/** Timeline controlling the door's movement */
	UPROPERTY(BlueprintReadOnly, Category = "Door|Timeline")
	TObjectPtr<UTimelineComponent> DoorTimeline;

	/** Direction of the door timeline */
	UPROPERTY(BlueprintReadOnly, Category = "Door|Timeline")
	TEnumAsByte<ETimelineDirection::Type> DoorTimelineDirection;

	/** Required or else Unreal yells at us in the console because we have SetObject on the time line to true and the float curve property to none */
	UPROPERTY()
	float DoorTimelineFloat;

	/** @} */

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UArrowComponent> ArrowComponent;
#endif

	/** Handles all movement code for the timeline */
	UFUNCTION()
	void DoorTimelineTick(const float Alpha);

	/** Handles setting various properties as the timeline logic happens async to the main code */
	UFUNCTION()
	void DoorTimelineCompleted();

	// These need to be UProperty for me to use them in OnConstruction, no idea why...
	UPROPERTY()
	FVector Door1OpenVector;

	UPROPERTY()
	FVector Door1CloseVector;

	UPROPERTY()
	FVector Door2OpenVector;

	UPROPERTY()
	FVector Door2CloseVector;
};
