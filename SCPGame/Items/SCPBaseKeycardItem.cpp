#include "Items/SCPBaseKeycardItem.h"

ASCPBaseKeycardItem::ASCPBaseKeycardItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASCPBaseKeycardItem::BeginPlay()
{
	Super::BeginPlay();
}

void ASCPBaseKeycardItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASCPBaseKeycardItem::OnItemUseBegin_Implementation(AActor* User)
{
	return User != nullptr;
}

void ASCPBaseKeycardItem::OnItemUseSuccess_Implementation(AActor* User)
{

}
