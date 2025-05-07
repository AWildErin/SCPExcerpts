#include "Actors/SCPBaseButton.h"

#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "FMODAudioComponent.h"
#include "FMODEvent.h"

#include "Misc/SCPPhysicsTypes.h"

ASCPBaseButton::ASCPBaseButton()
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

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>("ButtonMesh");
	ButtonMesh->SetupAttachment(RootComponent);

	ButtonAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>("ButtonAudioComponent");
	ButtonAudioComponent->SetupAttachment(RootComponent);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>("InteractionSphere");
	InteractionSphere->SetSphereRadius(32.f);
	InteractionSphere->SetCollisionProfileName(FSCPCollisionPresets::Preset_Interactable);
	InteractionSphere->SetupAttachment(RootComponent);
}

void ASCPBaseButton::BeginPlay()
{
	Super::BeginPlay();
}

void ASCPBaseButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCPBaseButton::OnInteract_Implementation(AActor* Interactor)
{
	OnButtonPressBegin.Broadcast(Interactor);

	if (!bMuteSounds && PressedSoundEvent)
	{
		ButtonAudioComponent->SetEvent(PressedSoundEvent);
		ButtonAudioComponent->Play();
	}

	if (!BeginPress(Interactor))
	{
		if (!bMuteSounds && FailedSoundEvent)
		{
			ButtonAudioComponent->SetEvent(FailedSoundEvent);
			ButtonAudioComponent->Play();
		}

		OnButtonPressFailed.Broadcast(Interactor);
		return;
	}

	FinishPress(Interactor);
}

bool ASCPBaseButton::BeginPress_Implementation(AActor* Interactor)
{
	return true;
}

void ASCPBaseButton::FinishPress(AActor* Interactor)
{
	if (!bMuteSounds && SuccessSoundEvent)
	{
		ButtonAudioComponent->SetEvent(SuccessSoundEvent);
		ButtonAudioComponent->Play();
	}

	OnButtonPressSuccess.Broadcast(Interactor);
}
