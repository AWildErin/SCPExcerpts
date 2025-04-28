#include "Interaction/SCPInteractActor.h"

ASCPInteractActor::ASCPInteractActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASCPInteractActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASCPInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

