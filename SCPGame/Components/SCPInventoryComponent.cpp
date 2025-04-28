#include "Components/SCPInventoryComponent.h"

#include "Logging/StructuredLog.h"

#include "Items/SCPBaseItem.h"
#include "SCPGameModule.h"

USCPInventoryComponent::USCPInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void USCPInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InventoryArray.AddZeroed(InventorySize);

	TObjectPtr<UWorld> World = GetWorld();
	check(World);

	TObjectPtr<AActor> Owner = GetOwner();
	checkf(Owner, TEXT("Failed to get inventory component owner!"));

	FVector OwnerLocation = Owner->GetActorLocation();
	for (auto& Row : StartingInventoryMap)
	{
		int SlotIndex = Row.SlotIndex;
		TSubclassOf<ASCPBaseItem> ObjectClass = Row.ObjectClass;

		FActorSpawnParameters Params;
		TObjectPtr<ASCPBaseItem> Item = World->SpawnActor<ASCPBaseItem>(ObjectClass, FTransform(OwnerLocation));
		if (!AddItem(Item, SlotIndex))
		{
			UE_LOGFMT(LogSCPItem, Error, "Failed to add {item} at slot index inside {this}! Destroying the item.", Item->GetName(), SlotIndex, GetName());
			Item->Destroy();

			continue;
		}

		// Notify the item we picked it up.
		Item->NativePickup(Owner);
	}
}

void USCPInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USCPInventoryComponent::AddItem(ASCPBaseItem* Item, int NewIndex)
{
	/**
	* @todo Handle removing item from the previous inventory
	*/

	// If the item is null, don't even bother with it. Or if the inventory already contains this exact item
	if (!Item || InventoryArray.Contains(Item))
	{
		return false;
	}

	UE_LOGFMT(LogSCPItem, Log, "Trying to add {item} to {inv}", Item->GetName(), GetName());

	int WantedIndex = NewIndex;

	// If we didn't request an index, or our index is already taken, find a new one
	if (WantedIndex == -1 || InventoryArray[WantedIndex] != nullptr)
	{
		WantedIndex = FindNextFreeIndex();
	}

	// If we got -1, we didn't find a free index, we don't add the item.
	if (WantedIndex == -1)
	{
		UE_LOGFMT(LogSCPItem, Log, "Failed to find an index for {item}. Wanted index {Wanted} NewIndex {New}", Item->GetName(), WantedIndex, NewIndex);
		return false;
	}

	InventoryArray[WantedIndex] = Item;
	Item->SetOwningInventory(this);

	OnItemAddedDelegate.Broadcast(Item, WantedIndex);

	return true;
}

bool USCPInventoryComponent::GiveItem(TSubclassOf<ASCPBaseItem> ItemClass, int Index)
{
	return false;
}

bool USCPInventoryComponent::RemoveItem(ASCPBaseItem* Item)
{
	if (!Item)
	{
		return false;
	}

	if (!InventoryArray.Contains(Item))
	{
		UE_LOGFMT(LogSCPItem, Log, "{item} wasn't in the inventory component", Item->GetName());
		return false;
	}

	UE_LOGFMT(LogSCPItem, Log, "Trying to remove {Item} from {Inventory}", Item->GetName(), GetName());

	int ItemIndex = InventoryArray.Find(Item);
	InventoryArray[ItemIndex] = nullptr;
	Item->SetOwningInventory(nullptr);

	OnItemRemovedDelegate.Broadcast(Item);

	return true;
}

bool USCPInventoryComponent::UseItem(AActor* User, ASCPBaseItem* Item)
{
	if (!User || !Item)
	{
		UE_LOGFMT(LogSCPItem, Log, "User or Item was null on UseItem!");
		return false;
	}

	return Item->OnItemUse(User);
}

ASCPBaseItem* USCPInventoryComponent::GetItemInSlot(int Slot)
{
	if (Slot > InventoryArray.Num())
	{
		return nullptr;
	}

	return InventoryArray[Slot];
}

bool USCPInventoryComponent::DropItem(ASCPBaseItem* Item)
{
	if (!RemoveItem(Item))
	{
		return false;
	}

	Item->NativeDrop(Item);

	return true;
}

bool USCPInventoryComponent::MoveItem(ASCPBaseItem* Item, int NewIndex)
{
	// Check if item exists
	if (!Item)
	{
		return false;
	}

	// Check if item belongs to us
	int ItemIndex = InventoryArray.Find(Item);
	if (ItemIndex == INDEX_NONE)
	{
		return false;
	}

	// Does this slot already contain an item
	if (InventoryArray[NewIndex])
	{
		return false;
	}

	InventoryArray[NewIndex] = Item;
	InventoryArray[ItemIndex] = nullptr;

	return true;
}

int USCPInventoryComponent::FindNextFreeIndex()
{
	for (int i = 0; i < InventorySize; i++)
	{
		if (InventoryArray[i] == nullptr)
		{
			return i;
		}
	}

	return -1;
}
