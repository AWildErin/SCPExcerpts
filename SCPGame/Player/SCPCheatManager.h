#pragma once

#include "GameFramework/CheatManager.h"

#include "SCPCheatManager.generated.h"

UCLASS(config = Game, Within = PlayerController, MinimalAPI)
class USCPCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(exec)
	virtual void TeleportToRoom(FString RoomName);

	UFUNCTION(exec)
	virtual void InfiniteStamina();
};
