#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Items/SCPBaseItem.h"

#include "SCPBaseItemTest.generated.h"

/**
* @class ASCPBaseKeycardItem
* Actor used within our tests
*/
UCLASS(NotBlueprintable)
class SCPGAME_API ASCPBaseItemTest : public ASCPBaseItem
{
	GENERATED_BODY()

public:
	ASCPBaseItemTest();

	// Begin AActor Interface
	virtual void BeginPlay() override;
	// End AActor Interface

	// Begin ASCPBaseItem Interface
	bool OnItemUseBegin_Implementation(AActor* User) override;
	void OnItemUseSuccess_Implementation(AActor* User) override;
	// End ASCPBaseItem Interface
};
