#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "SCPInventoryComponent.generated.h"

class ASCPBaseItem;

/** Class exists to work around TMultiMap not working in BP, and TPair ALSO not working in BP.*/
USTRUCT(BlueprintType)
struct FSCPStartingInventoryRow
{
	GENERATED_BODY();

	/** Slot to place the item in */
	UPROPERTY(EditDefaultsOnly)
	int SlotIndex = -1;

	/** Item class to spawn*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASCPBaseItem> ObjectClass;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAddedSignature, ASCPBaseItem*, Item, int, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemovedSignature, ASCPBaseItem*, Item);

/**
 * @todo Add item pickup sounds
 * @todo equipping
 * @todo Toast manager so we can print messages to the user about being unable to pick up item
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SCPGAME_API USCPInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USCPInventoryComponent();

	// Begin UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// End UActorComponent Interface

	/// @custom_region{UFunctions}

	/**
	 * Adds the given item to the inventory array, with the option of placing into a specific index.
	 * If the requested index is -1, the item will be placed into the next free slot, if the requested index
	 * is not empty, the item will be placed into the next free slot.
	 *
	 * @param Item		Item we want to add
	 * @param NewIndex	Specific index we want to add it to.
	 * @returns Whether or not the item could be added to the inventory.
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(ASCPBaseItem* Item, int NewIndex = -1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool GiveItem(TSubclassOf<ASCPBaseItem> ItemClass, int Index = -1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(ASCPBaseItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(AActor* User, ASCPBaseItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	ASCPBaseItem* GetItemInSlot(int Slot);

	/** Drops the item from the inventory, different as RemoveItem does not re-enable the item mesh or make it physical */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool DropItem(ASCPBaseItem* Item);

	/** Moves item between slots in the current inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool MoveItem(ASCPBaseItem* Item, int NewIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<ASCPBaseItem*> GetInventoryArray() { return InventoryArray; }

	/// @}

	/// @custom_region{UProperties}

	/**
	 * Total number of slots in the inventory.
	 * @note This is the TOTAL. Not taking into account array indexing starting at 0. For loops, - 1 from the number
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	int InventorySize;

	/** Starting inventory item map. Spawns items upon component creation. Index can be -1 to fill the next open slot. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TArray<FSCPStartingInventoryRow> StartingInventoryMap;

	/// @}

	/// @custom_region{Delegates}

	/** Called when an item is added to the inventory */
	UPROPERTY(BlueprintAssignable)
	FOnItemAddedSignature OnItemAddedDelegate;

	/** Called when an item is removed to the inventory. Does not always mean the item was dropped. */
	UPROPERTY(BlueprintAssignable)
	FOnItemRemovedSignature OnItemRemovedDelegate;

	/// @}

private:
	/** Finds the next free index within the inventory array */
	int FindNextFreeIndex();

	TArray<ASCPBaseItem*> InventoryArray;
};
