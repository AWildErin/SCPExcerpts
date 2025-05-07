#include "Actors/SCPDoorFactory.h"

#include "Logging/StructuredLog.h"
#include "Components/ChildActorComponent.h"

#include "SCPBaseDoor.h"
#include "SCPGameModule.h"

ASCPDoorFactory::ASCPDoorFactory()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	// Create button components
	LeftButtonComponent = CreateDefaultSubobject<UChildActorComponent>("LeftButton");
	LeftButtonComponent->SetupAttachment(RootComponent);

	RightButtonComponent = CreateDefaultSubobject<UChildActorComponent>("RightButton");
	RightButtonComponent->SetupAttachment(RootComponent);

	// Create door components
	DoorComponent = CreateDefaultSubobject<UChildActorComponent>("Door");
	DoorComponent->SetupAttachment(RootComponent);

	if (ASCPBaseDoor* Door = Cast<ASCPBaseDoor>(DoorComponent->GetChildActor()))
	{
		/** @todo This functionality isn't in just yet! */
		Door->bSpawnsOpen = bStartOpen;
		ensureMsgf(bStartOpen == false,
				   TEXT("%s has defined bStartOpen! as of 17/09/2024, this is not currently implemented. Implement it then remove this!!!"), *GetNameSafe(this));
	}
}

void ASCPDoorFactory::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Spawn Buttons
	TSubclassOf<ASCPBaseButton> ButtonClass;
	if (ButtonTypeMap.Contains(SpawnButtonType))
	{
		ButtonClass = ButtonTypeMap[SpawnButtonType];
	}

	if (ButtonClass != nullptr && LeftButtonComponent)
	{
		LeftButtonComponent->SetChildActorClass(ButtonClass);
	}
	else
	{
		LeftButtonComponent->SetChildActorClass(nullptr);
	}

	if (ButtonClass != nullptr && RightButtonComponent)
	{
		RightButtonComponent->SetChildActorClass(ButtonClass);
	}
	else
	{
		RightButtonComponent->SetChildActorClass(nullptr);
	}

	// If we're null, just exit early from the function since we can't correctly set the door in that case
	if (DoorClass == nullptr)
	{
		UE_LOGFMT(LogSCPMap, Error, "{0}'s DoorClass was null!", GetNameSafe(this));
		return;
	}

	if (DoorComponent)
	{
		DoorComponent->SetChildActorClass(DoorClass);
	}
}

void ASCPDoorFactory::BeginPlay()
{
	Super::BeginPlay();

	if (DoorComponent)
	{
		ASCPBaseDoor* Door = Cast<ASCPBaseDoor>(DoorComponent->GetChildActor());

		if (ASCPBaseButton* Button = Cast<ASCPBaseButton>(LeftButtonComponent->GetChildActor()))
		{
			Button->OnButtonPressSuccess.AddDynamic(this, &ThisClass::OnButtonSuccess);
		}

		if (ASCPBaseButton* Button = Cast<ASCPBaseButton>(RightButtonComponent->GetChildActor()))
		{
			Button->OnButtonPressSuccess.AddDynamic(this, &ThisClass::OnButtonSuccess);
		}
	}
}

void ASCPDoorFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCPDoorFactory::OnButtonSuccess(AActor* Interactor)
{
	ASCPBaseDoor* Door = Cast<ASCPBaseDoor>(DoorComponent->GetChildActor());
	if (Door)
	{
		Door->ToggleDoor();
	}
}
