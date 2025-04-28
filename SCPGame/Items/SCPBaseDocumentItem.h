#pragma once

#include "CoreMinimal.h"

#include "Items/SCPBaseItem.h"

#include "SCPBaseDocumentItem.generated.h"

/**
* @class ASCPBaseDocumentItem
* A Base class for all items that are an inventory
*/
UCLASS()
class SCPGAME_API ASCPBaseDocumentItem : public ASCPBaseItem
{
	GENERATED_BODY()

public:
	ASCPBaseDocumentItem();

	// Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// End AActor Interface

	// Begin ASCPBaseItem Interface
	bool OnItemUseBegin_Implementation(AActor* User) override;
	void OnItemUseSuccess_Implementation(AActor* User) override;
	// End ASCPBaseItem Interface
};
