#pragma once

#include "GameFramework/CheatManager.h"

#include "SCPCheatManager.generated.h"

struct FAutoCompleteCommand;
class ASCPBaseItem;

UCLASS(config = Game, Within = PlayerController, MinimalAPI)
class USCPCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	// Begin UCheatManager Interface
	void InitCheatManager() override;
	// End UCheatManager Interface

	// Begin UObject Interface
	void BeginDestroy() override;
	// End UObject Interface

	UFUNCTION(exec)
	virtual void TeleportToRoom(FString RoomName);

	UFUNCTION(exec)
	virtual void InfiniteStamina();

	/** @custom_region{Spawners} */
	UFUNCTION(exec)
	virtual void SpawnItem(FString ItemPath);

	UFUNCTION(exec)
	virtual void Spawn(FString NPCName);
	/** @} */

	/**
	 * In shipping and test, this is called by FSCPGameModule
	 * In development/editor, this is called by USCPGameInstance.
	 */
	static void AddConsoleAutoCompletes(TArray<FAutoCompleteCommand>& CommandList);

private:
	static TArray<TObjectPtr<UClass>> GetAllItemClasses();
	TArray<TObjectPtr<UClass>> CachedItemClasses;
	TMap<FName, TObjectPtr<UClass>> CachedItemCheatNameMap;
};
