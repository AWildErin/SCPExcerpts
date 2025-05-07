#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SCPEntitySubsystem.generated.h"

class ASCPCharacter;

/**
 * @class USCPEntitySubsystem
 * The subsystem that handles spawning instances of an SCP entity. (e.g. 096, 173, 106)
 *
 * @todo Make a custom character or pawn class that we can use in place of here, just so we don't attempt to spawn guards or the player with this.
 */
UCLASS(BlueprintType, meta = (DisplayName = "SCP Entity Subsystem"))
class USCPEntitySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem Interface

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = CharacterClass))
	ASCPCharacter* SpawnInstance(TSubclassOf<ASCPCharacter> CharacterClass, FName InstanceName, FVector SpawnLocation = FVector::ZeroVector,
								 FRotator SpawnRotation = FRotator::ZeroRotator);

	/**
	 * Summons the entity to the given location
	 * @param InstanceName	Name of the entity instance to summon
	 * @param SummonLocation	Vector to teleport the entity to
	 * @param SummonRotation	Optional rotation
	 * @param bKeepRotation	Whether or not to keep the current rotation of the entity when summoning. If false, SummonRotation is used.
	 */
	UFUNCTION(BlueprintCallable)
	ASCPCharacter* SummonInstance(FName InstanceName, FVector SummonLocation, FRotator SummonRotation = FRotator::ZeroRotator, bool bKeepRotation = true);

	/** Functionally identical to SummonInstance but intead will spawn the instance if it was not found. */
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = Class))
	ASCPCharacter* SummonOrSpawn(TSubclassOf<ASCPCharacter> Class, FName InstanceName, FVector SummonLocation, FRotator SummonRotation = FRotator::ZeroRotator,
								 bool bKeepRotation = true);

	UFUNCTION(BlueprintCallable)
	ASCPCharacter* GetInstance(FName InstanceName);

	/** Functionally identical to GetInstance but instead will spawn the instance if it was not found. */
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = Class))
	ASCPCharacter* GetOrSpawn(TSubclassOf<ASCPCharacter> Class, FName InstanceName, FVector SpawnLocation = FVector::ZeroVector, FRotator SpawnRotation = FRotator::ZeroRotator);

	/** Returns whether or not the Entity Subsystem has spawned the specified entity */
	UFUNCTION(BlueprintCallable)
	bool InstanceExists(FName InstanceName) { return SpawnedInstances.Contains(InstanceName); }

	void DestroyInstance(FName InstanceName);

private:
	TMap<FName, TObjectPtr<ASCPCharacter>> SpawnedInstances;
};
