#pragma once

#include "CoreMinimal.h"

#include "Items/SCPBaseItem.h"

#include "SCPBaseKeycardItem.generated.h"

/**
* @class ASCPBaseKeycardItem
* A Base class for all items that are an inventory
*/
UCLASS()
class SCPGAME_API ASCPBaseKeycardItem : public ASCPBaseItem
{
	GENERATED_BODY()

public:
	ASCPBaseKeycardItem();

	// Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// End AActor Interface

	// Begin ASCPBaseItem Interface
	bool OnItemUseBegin_Implementation(AActor* User) override;
	void OnItemUseSuccess_Implementation(AActor* User) override;
	// End ASCPBaseItem Interface
};
