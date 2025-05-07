#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interaction/SCPInteractInterface.h"
#include "Data/SCPItemDataAsset.h"

#include "SCPBaseItem.generated.h"

class USphereComponent;

class USCPInventoryComponent;

/**
* @class ASCPBaseItem
* @todo Handle item collisions with other items and fake gravity on them like original CB
*/
UCLASS(Abstract)
class SCPGAME_API ASCPBaseItem
	: public AActor
	, public ISCPInteractInterface
{
	GENERATED_BODY()

public:
	ASCPBaseItem();

	// Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// End AActor Interface

	// Begin ISCPInteractInterface
	bool RequiresFocus_Implementation() override { return false; }
	bool CanInteract_Implementation(AActor* Interactor) override { return !bIsInInventory; }
	void OnInteract_Implementation(AActor* Interactor) override;
	// End ISCPInteractInterface

	/// @custom_region{Components}
	/** Returns the owning inventory for this item, keep in mind this may be null!*/
	UFUNCTION(BlueprintCallable, Category = "Item")
	USCPInventoryComponent* GetOwningInventory() const { return OwningInventory; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetOwningInventory(USCPInventoryComponent* NewInventory) { OwningInventory = NewInventory; }
	///

	/// @custom_region{API}
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void OnItemDropped(AActor* Interactor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void OnItemPickedUp(AActor* Interactor);

	/**
	 * Fires when an item is dragged and dropped onto us.
	 * Overwrite the implementation function if you need to write native code for this item.
	 *
	 * @param OtherItem	The item dropped onto us.
	 * @returns Whether or not the drop was successful
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	bool OnItemDroppedOnto(ASCPBaseItem* OtherItem);
	/** @private */ virtual bool OnItemDroppedOnto_Implementation(ASCPBaseItem* OtherItem);

	/**
	* Fires off the entire Use chain
	* @returns Whether or not the use was successful
	*/
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool OnItemUse(AActor* User);

	/**
	* @returns whether or not the item was successfully used
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Item|Internal")
	bool OnItemUseBegin(AActor* User);
	/** @private */ virtual bool OnItemUseBegin_Implementation(AActor* User);

	/**
	 * Fires when OnItemUseBegin returns true
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Item|Internal")
	void OnItemUseSuccess(AActor* User);
	/** @private */ virtual void OnItemUseSuccess_Implementation(AActor* User);

	/**
	 * Fires when OnItemUseBegin returns false
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Item|Internal")
	void OnItemUseFail(AActor* User);
	/** @private */ virtual void OnItemUseFail_Implementation(AActor* User);
	/// @}

	/**
	* Handles some native setup when we pickup the item
	*
	* @todo Maybe take inventory as well so we are parented to the owner of that instead.
	*/
	virtual void NativePickup(AActor* Interactor);

	/** Handles some native setup when we drop the item */
	virtual void NativeDrop(AActor* Interactor);

	/** Handles basic checks for interaction if we're in an inventory. */
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	bool bIsInInventory;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item")
	TObjectPtr<USCPItemDataAsset> ItemData;

protected:
	/// @custom_region{Components}
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	/** The inventory this item is currently in */
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	TObjectPtr<USCPInventoryComponent> OwningInventory;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item")
	TObjectPtr<USphereComponent> InteractionSphere;
	/// @}
};
