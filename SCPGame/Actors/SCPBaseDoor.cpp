#include "Actors/SCPBaseDoor.h"

#include "Logging/StructuredLog.h"
#include "Components/ArrowComponent.h"
#include "FMODAudioComponent.h"
#include "FMODEvent.h"

#include "SCPGameModule.h"

ASCPBaseDoor::ASCPBaseDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(255, 0, 0);
		ArrowComponent->SetupAttachment(RootComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
	}
#endif

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrameMesh");
	DoorFrameMesh->SetupAttachment(RootComponent);

	Door1Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Door1Mesh");
	Door1Mesh->SetupAttachment(RootComponent);

	Door1OpenPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Door1OpenPosition"));
	Door1OpenPosition->SetupAttachment(RootComponent);

	Door2Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Door2Mesh");
	Door2Mesh->SetupAttachment(RootComponent);

	Door2OpenPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Door2OpenPosition"));
	Door2OpenPosition->SetupAttachment(RootComponent);

	// Audio
	DoorSoundComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("DoorSoundComponent"));
	DoorSoundComponent->bAutoActivate = false;
	DoorSoundComponent->SetupAttachment(RootComponent);
}

void ASCPBaseDoor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Door1OpenVector = Door1OpenPosition->GetRelativeLocation();
	Door1CloseVector = Door1Mesh->GetRelativeLocation();

	Door2OpenVector = Door2OpenPosition->GetRelativeLocation();
	Door2CloseVector = Door2Mesh->GetRelativeLocation();

	DoorFrameMesh->SetVisibility(!bHideFrame);

	/** @todo Handle start open. If we do this we can't move the door at all even when bSpawnsOpen is false. */
	// FVector Door1Position = bSpawnsOpen ? Door1OpenVector : Door1CloseVector;
	// FVector Door2Position = bSpawnsOpen ? Door2OpenVector : Door2CloseVector;
	// Door1Mesh->SetRelativeLocation(Door1Position);
	// Door2Mesh->SetRelativeLocation(Door2Position);
}

void ASCPBaseDoor::BeginPlay()
{
	Super::BeginPlay();

	// Setup door timeline
	if (DoorOpenCurve)
	{
		FOnTimelineFloat DoorTimelineTick;
		DoorTimelineTick.BindUFunction(this, FName("DoorTimelineTick"));

		// Despite never using it, we must add the last param if we use SetPropertySetObject.
		// We use it to ONLY to get the direction, but the engine yells at us when we play the timeline
		// because the float parameter was set to NAME_None....
		DoorTimeline->AddInterpFloat(DoorOpenCurve, DoorTimelineTick, "DoorTimelineFloat");

		FOnTimelineEvent DoorTimelineCompleted;
		DoorTimelineCompleted.BindUFunction(this, FName("DoorTimelineCompleted"));

		// Required so we can get the direction
		DoorTimeline->SetPropertySetObject(this);
		DoorTimeline->SetDirectionPropertyName(FName("DoorTimelineDirection"));

		DoorTimeline->SetTimelineLengthMode(TL_LastKeyFrame);
		DoorTimeline->SetTimelineFinishedFunc(DoorTimelineCompleted);
	}
}

void ASCPBaseDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCPBaseDoor::OpenDoor(bool bFromStart /*= true*/, bool bMuteSound /*= false*/)
{
	if (bIsMoving)
	{
		//return;
	}

	OnDoorOpening.Broadcast(this);

	bIsMoving = true;

	if (OpenSoundEvent)
	{
		DoorSoundComponent->SetEvent(OpenSoundEvent);
		DoorSoundComponent->Play();
	}
	else
	{
		UE_LOGFMT(LogSCPMap, Warning, "OpenSoundEvent was invalid on {name}!", GetNameSafe(this));
	}

	if (bFromStart)
	{
		DoorTimeline->PlayFromStart();
	}
	else
	{
		DoorTimeline->Play();
	}
}

void ASCPBaseDoor::CloseDoor(bool bFromEnd /*= true*/, bool bMuteSound /*= false*/)
{
	if (bIsMoving)
	{
		//return;
	}

	OnDoorClosing.Broadcast(this);

	bIsMoving = true;

	if (CloseSoundEvent && !bMuteSound)
	{
		DoorSoundComponent->SetEvent(CloseSoundEvent);
		DoorSoundComponent->Play();
	}
	else
	{
		UE_LOGFMT(LogSCPMap, Warning, "CloseSoundEvent was invalid on {name}!", GetNameSafe(this));
	}

	if (bFromEnd)
	{
		DoorTimeline->ReverseFromEnd();
	}
	else
	{
		DoorTimeline->Reverse();
	}

}

void ASCPBaseDoor::ToggleDoor(bool bFromStartOrEnd /*= true*/, bool bMuteSound /*= false*/)
{
	if (bIsMoving)
	{
		return;
	}

	if (bIsOpen)
	{
		CloseDoor(bFromStartOrEnd, bMuteSound);
	}
	else
	{
		OpenDoor(bFromStartOrEnd, bMuteSound);
	}
}

void ASCPBaseDoor::DoorTimelineTick(const float Alpha)
{
	FVector Door1LerpVector = FMath::Lerp(Door1CloseVector, Door1OpenVector, Alpha);
	Door1Mesh->SetRelativeLocation(Door1LerpVector);

	FVector Door2LerpVector = FMath::Lerp(Door2CloseVector, Door2OpenVector, Alpha);
	Door2Mesh->SetRelativeLocation(Door2LerpVector);
}

void ASCPBaseDoor::DoorTimelineCompleted()
{
	OnDoorCompleted.Broadcast(this);

	bIsMoving = false;

	if (DoorTimelineDirection == ETimelineDirection::Type::Forward)
	{
		bIsOpen = true;
	}
	else
	{
		bIsOpen = false;
	}
}
