#pragma once

#include "CoreMinimal.h"

#include "Items/SCPBaseItem.h"

#include "SCPBaseContainerItem.generated.h"

class USCPInventoryComponent;

/**
* @class ASCPBaseContainerItem
* A Base class for all items that are an inventory
*/
UCLASS()
class SCPGAME_API ASCPBaseContainerItem : public ASCPBaseItem
{
	GENERATED_BODY()

public:
	ASCPBaseContainerItem();

	// Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// End AActor Interface

	// Begin ASCPBaseItem Interface
	virtual bool OnItemDroppedOnto_Implementation(ASCPBaseItem* OtherItem);
	bool OnItemUseBegin_Implementation(AActor* User) override;
	void OnItemUseSuccess_Implementation(AActor* User) override;
	// End ASCPBaseItem Interface

	// Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item|Components")
	USCPInventoryComponent* InventoryComponent;

	/** Dictates what items are allowed in this item container */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	TArray<TSubclassOf<ASCPBaseItem>> AllowedItemClassesList;
};
