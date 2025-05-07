#include "Subsystems/SCPEntitySubsystem.h"

#include "Logging/StructuredLog.h"

#include "Character/SCPCharacter.h"
#include "SCPGameModule.h"

void USCPEntitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USCPEntitySubsystem::Deinitialize()
{
	Super::Deinitialize();
}

ASCPCharacter* USCPEntitySubsystem::SpawnInstance(TSubclassOf<ASCPCharacter> CharacterClass, FName InstanceName,
												  FVector SpawnLocation /*= FVector::ZeroVector*/, FRotator SpawnRotation /*= FRotator::ZeroRotator*/)
{
	if (InstanceExists(InstanceName))
	{
		UE_LOGFMT(LogSCPMap, Error, "[{0}] An instance with the name '{1}' already exists!", GetNameSafe(this), InstanceName);
		return nullptr;
	}

	TObjectPtr<UWorld> World = GetWorld();
	checkf(World, TEXT("Failed to get the world for USCPEntitySubsystem when trying to spawn %s '%s'!"), *GetNameSafe(this), *InstanceName.ToString());

	ASCPCharacter* Character = World->SpawnActor<ASCPCharacter>(CharacterClass, SpawnLocation, SpawnRotation);
	if (!Character)
	{
		UE_LOGFMT(LogSCPMap, Error, "[{0} (SpawnInstance)] Failed to spawn {1} '{2}'.", GetNameSafe(this), GetNameSafe(CharacterClass), InstanceName);
		UE_DEBUG_BREAK();
		return nullptr;
	}

	SpawnedInstances.Add(InstanceName, Character);

	return Character;
}

ASCPCharacter* USCPEntitySubsystem::SummonInstance(FName InstanceName, FVector SummonLocation, FRotator SummonRotation /*= FRotator::ZeroRotator*/, bool bKeepRotation /*= true*/)
{
	if (!InstanceExists(InstanceName))
	{
		UE_LOGFMT(LogSCPMap, Error, "[{0}] Failed to find spawned '{1}' instance. Please make sure it's spawned, or use SummonOrSpawn.", GetNameSafe(this), InstanceName);
		return nullptr;
	}

	TObjectPtr<ASCPCharacter> Character = SpawnedInstances[InstanceName];
	Character->TeleportTo(SummonLocation, Character->GetActorRotation());
	if (!bKeepRotation)
	{
		Character->SetActorRotation(SummonRotation);
	}

	return Character;
}

ASCPCharacter* USCPEntitySubsystem::SummonOrSpawn(TSubclassOf<ASCPCharacter> Class, FName InstanceName, FVector SummonLocation,
												  FRotator SummonRotation /*= FRotator::ZeroRotator*/, bool bKeepRotation /*= true*/)
{
	if (!InstanceExists(InstanceName))
	{
		if (!SpawnInstance(Class, InstanceName, FVector::ZeroVector, FRotator::ZeroRotator))
		{
			UE_LOGFMT(LogSCPMap, Error, "[{0} (SpawnOrSummon)] Failed to spawn {1} '{2}'.", GetNameSafe(this), GetNameSafe(Class), InstanceName);
			return nullptr;
		}
	}

	return SummonInstance(InstanceName, SummonLocation, SummonRotation, bKeepRotation);
}

ASCPCharacter* USCPEntitySubsystem::GetInstance(FName InstanceName)
{
	if (!InstanceExists(InstanceName))
	{
		UE_LOGFMT(LogSCPMap, Error, "[{0}] Failed to find spawned '{1}' instance. Please make sure it's spawned, or use GetOrSpawn.", GetNameSafe(this), InstanceName);
		return nullptr;
	}

	return SpawnedInstances[InstanceName];
}

ASCPCharacter* USCPEntitySubsystem::GetOrSpawn(TSubclassOf<ASCPCharacter> Class, FName InstanceName, FVector SpawnLocation /*= FVector::ZeroVector*/,
											   FRotator SpawnRotation /*= FRotator::ZeroRotator*/)
{
	if (!InstanceExists(InstanceName))
	{
		if (!SpawnInstance(Class, InstanceName, FVector::ZeroVector, FRotator::ZeroRotator))
		{
			UE_LOGFMT(LogSCPMap, Error, "[{0} (GetOrSpawn)] Failed to spawn {1} '{2}'.", GetNameSafe(this), GetNameSafe(Class), InstanceName);
			return nullptr;
		}
	}

	return GetInstance(InstanceName);
}

void USCPEntitySubsystem::DestroyInstance(FName InstanceName)
{
	TObjectPtr<ASCPCharacter> Character = GetInstance(InstanceName);
	if (!Character)
	{
		return;
	}

	Character->Destroy();
	SpawnedInstances.Remove(InstanceName);
}
