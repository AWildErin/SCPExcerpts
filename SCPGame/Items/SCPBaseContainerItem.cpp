#include "Items/SCPBaseContainerItem.h"

#include "Logging/StructuredLog.h"

#include "Player/SCPPlayerCharacter.h"
#include "Components/SCPInventoryComponent.h"
#include "SCPGameModule.h"

ASCPBaseContainerItem::ASCPBaseContainerItem()
{
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<USCPInventoryComponent>("InventoryComponent");
	InventoryComponent->InventorySize = 10;
}

void ASCPBaseContainerItem::BeginPlay()
{
	Super::BeginPlay();
}

void ASCPBaseContainerItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASCPBaseContainerItem::OnItemUseBegin_Implementation(AActor* User)
{
	return User && InventoryComponent;
}

void ASCPBaseContainerItem::OnItemUseSuccess_Implementation(AActor* User)
{
	if (TObjectPtr<ASCPPlayerCharacter> Character = Cast<ASCPPlayerCharacter>(User))
	{
		Character->CreateInventoryInterface(InventoryComponent);
	}
}

bool ASCPBaseContainerItem::OnItemDroppedOnto_Implementation(ASCPBaseItem* OtherItem)
{
	if (!OtherItem || !OwningInventory)
	{
		return false;
	}

	UE_LOGFMT(LogSCPItem, Verbose, "Trying to add {item} onto {us}", OtherItem->GetName(), GetName());

	// Check to see if we're allowed to place the item inside our container
	auto Pred = [&OtherItem](TSubclassOf<ASCPBaseItem>& ArrayItem) { return OtherItem->IsA(ArrayItem); };
	if (AllowedItemClassesList.Num() > 0 && !AllowedItemClassesList.ContainsByPredicate(Pred))
	{
		UE_LOGFMT(LogSCPItem, Verbose, "{item} did not pass the allowed check for {us}", OtherItem->GetName(), GetName());
		return false;
	}

	// Store reference so we can remove the item from it later
	USCPInventoryComponent* OtherOwningInventory = OtherItem->GetOwningInventory();

	// Try to add our item
	if (!InventoryComponent->AddItem(OtherItem))
	{
		UE_LOGFMT(LogSCPItem, Verbose, "Failed to add {item} into {us}", OtherItem->GetName(), GetName());
		return false;
	}

	// Remove item from the other inventory
	if (!OtherOwningInventory->RemoveItem(OtherItem))
	{
		UE_LOGFMT(LogSCPItem, Verbose, "Failed to remove {item} from {invcomp}", OtherItem->GetName(), OtherOwningInventory->GetName());
		return false;
	}

	// Handle ownership change
	OtherItem->NativePickup(this);

	return true;
}
