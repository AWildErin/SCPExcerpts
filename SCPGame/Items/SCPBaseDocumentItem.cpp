#include "Items/SCPBaseDocumentItem.h"

ASCPBaseDocumentItem::ASCPBaseDocumentItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASCPBaseDocumentItem::BeginPlay()
{
	Super::BeginPlay();
}

void ASCPBaseDocumentItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASCPBaseDocumentItem::OnItemUseBegin_Implementation(AActor* User)
{
	return User != nullptr;
}

void ASCPBaseDocumentItem::OnItemUseSuccess_Implementation(AActor* User)
{

}
