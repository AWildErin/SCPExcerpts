#include "Player/SCPCheatManager.h"

#include "Engine/Console.h"
#include "EngineUtils.h"
#include "Logging/StructuredLog.h"

#include "Items/SCPBaseItem.h"
#include "Map/Generators/SCPCBGenerator.h"
#include "Map/SCPMapGenerator.h"
#include "Player/SCPPlayerCharacter.h"
#include "Player/SCPPlayerController.h"
#include "SCPGameModule.h"

void USCPCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

	CachedItemClasses = GetAllItemClasses();

	for (auto& Class : CachedItemClasses)
	{
		TObjectPtr<ASCPBaseItem> Item = Class->GetDefaultObject<ASCPBaseItem>();
		if (!Item || !Item->ItemData || Item->ItemData->CheatName.IsNone())
		{
			UE_LOGFMT(LogSCPCheat, Error, "[USCPCheatManager::InitCheatManager] Item for class {0} had an invalid CDO, item data or invalid cheat name! Skipping...",
					  GetNameSafe(Class));
			continue;
		}

		CachedItemCheatNameMap.Add(Item->ItemData->CheatName, Class);
	}
}

void USCPCheatManager::BeginDestroy()
{
	CachedItemClasses.Empty();
	CachedItemCheatNameMap.Empty();

	Super::BeginDestroy();
}

void USCPCheatManager::TeleportToRoom(FString RoomName)
{
	// Room name needs to be valid for us to teleport to it
	if (RoomName.IsEmpty())
	{
		UE_LOGFMT(LogSCPCheat, Error, "RoomName was empty!");
		return;
	}

	// Sanity check that we have a world and pawn.
	TObjectPtr<UWorld> World = GetPlayerController()->GetWorld();
	TObjectPtr<APawn> Pawn = GetPlayerController()->GetPawn();
	if (!World || !Pawn)
	{
		UE_LOGFMT(LogSCPCheat, Error, "Failed to get world or pawn for the current player controller!");
		return;
	}

	// Get the CB generator
	/** @todo This will need to be updated once we actively use other map generators */
	TObjectPtr<USCPCBGenerator> CBGenerator = nullptr;
	for (TActorIterator<ASCPMapGenerator> It(World); It; ++It)
	{
		TObjectPtr<ASCPMapGenerator> GeneratorActor = *It;
		TObjectPtr<USCPBaseGenerator> GeneratorObject = GeneratorActor->GetGenerator();
		if (GeneratorObject != nullptr && GeneratorObject->IsA<USCPCBGenerator>())
		{
			CBGenerator = Cast<USCPCBGenerator>(GeneratorObject);
			break;
		}
	}

	if (CBGenerator == nullptr)
	{
		UE_LOGFMT(LogSCPCheat, Error, "Failed to find a map generator using USCPCBGenerator. Currently this command only supports the CB map generator.");
		return;
	}

	// Find a room with the given name
	FVector Location = Pawn->GetActorLocation();
	bool bFoundRoom = false;
	auto MapGrid = CBGenerator->GetMapArray();
	FIntVector2 MapSize = CBGenerator->GetMapSize();

	/** @todo When we fix RoomEntries not having their location set, update this to use RoomEntries too*/
	for (int Y = 0; Y < MapSize.Y; Y++)
	{
		for (int X = 0; X < MapSize.X; X++)
		{
			FSCPCBRoomEntry& Room = MapGrid[X][Y];
			if (Room.RoomName != NAME_None && Room.RoomName == FName(RoomName))
			{
				Location = Room.WorldLocation;
				bFoundRoom = true;
				break;
			}
		}
	}

	if (!bFoundRoom)
	{
		UE_LOGFMT(LogSCPCheat, Error, "Could not find a room with the name \"{0}\"!", RoomName);
		return;
	}

	// Move the location up a bit so we don't get stuck
	Location += FVector(0.f, 0.f, 128.f);

	// Teleport the player pawn to that room
	Pawn->SetActorLocation(Location, false);
	UE_LOGFMT(LogSCPCheat, Display, "Teleporting {0} to {1} ({2})", Pawn->GetName(), RoomName, Location.ToString());
}

void USCPCheatManager::InfiniteStamina()
{
	TObjectPtr<APawn> Pawn = GetPlayerController()->GetPawn();
	TObjectPtr<ASCPPlayerCharacter> Player = Cast<ASCPPlayerCharacter>(Pawn);

	if (!Player)
	{
		UE_LOGFMT(LogSCPCheat, Error, "Failed to cast to player!");
		return;
	}

	Player->bInfiniteStamina = !Player->bInfiniteStamina;
	UE_LOGFMT(LogSCPCheat, Display, "Infinite stamina cheat is now {val}", Player->bInfiniteStamina);
}

void USCPCheatManager::SpawnItem(FString ItemPath)
{
	bool bIsClassPath = ItemPath.StartsWith("/Game/") && ItemPath.EndsWith("_C");

	// If we have a direct class path, use that
	TObjectPtr<UClass> ItemClass;
	if (bIsClassPath)
	{
		ItemClass = StaticLoadClass(ASCPBaseItem::StaticClass(), nullptr, *ItemPath);
	}
	else
	{
		FName ItemName = FName(ItemPath);
		if (CachedItemCheatNameMap.Contains(ItemName))
		{
			ItemClass = CachedItemCheatNameMap[ItemName];
		}
	}

	if (!ItemClass)
	{
		UE_LOGFMT(LogSCPCheat, Error, "Failed to find item with name/path {0}! Please make sure it's entered correctly.", ItemPath);
		return;
	}

	// Get the players eye location
	TObjectPtr<APlayerController> Controller = GetPlayerController();
	TObjectPtr<APawn> Pawn = Controller->GetPawn();

	FVector SpawnLocation = Pawn->GetActorLocation() + (Pawn->GetActorUpVector() * 150.f);
	FRotator SpawnRotation = FRotator(0.f, Pawn->GetActorRotation().Yaw, 0.f);

	TObjectPtr<ASCPBaseItem> SpawnedItemActor = GetWorld()->SpawnActor<ASCPBaseItem>(ItemClass, SpawnLocation, SpawnRotation);
	if (!SpawnedItemActor)
	{
		UE_LOGFMT(LogSCPCheat, Error, "Failed to spawn item from class {0}!", GetNameSafe(ItemClass));
		return;
	}
}

void USCPCheatManager::Spawn(FString NPCName) {}

void USCPCheatManager::AddConsoleAutoCompletes(TArray<FAutoCompleteCommand>& CommandList)
{
	FColor CheatColor(200, 200, 200, 255);

	FString SpawnItemFunctionName = GET_FUNCTION_NAME_CHECKED_OneParam(USCPCheatManager, SpawnItem, FString).ToString();

	UE_LOG(LogSCPCheat, Verbose, TEXT("Adding USCPCheatManager auto completes..."));

	// SpawnItem
	{
		TArray<TObjectPtr<UClass>> Classes = GetAllItemClasses();

		for (TObjectPtr<UClass> Class : Classes)
		{
			TObjectPtr<ASCPBaseItem> Item = Class->GetDefaultObject<ASCPBaseItem>();

			if (!Item || !Item->ItemData)
			{
				UE_LOGFMT(LogSCPCheat, Error, "Failed to get ASCPBaseItem or it's item data from class: {0}", GetNameSafe(Class));
				continue;
			}

			FString ItemName = Item->ItemData->ItemName.ToString();

			FString CommandStr = FString::Format(TEXT("{0} \"{1}\""), {SpawnItemFunctionName, Class->GetPathName()});
			UE_LOGFMT(LogSCPCheat, Verbose, "Adding {0} to auto complete", CommandStr);

			CommandList.AddDefaulted();
			FAutoCompleteCommand& Command = CommandList.Last();
			Command.Command = CommandStr;
			Command.Desc = FString::Printf(TEXT("Spawns '%s' item at the players location"), *ItemName);
			Command.Color = CheatColor;
		}
	}
}

TArray<TObjectPtr<UClass>> USCPCheatManager::GetAllItemClasses()
{
	TArray<TObjectPtr<UClass>> Classes;

	for (TObjectPtr<UClass> Class : TObjectRange<UClass>())
	{
		FString ClassName = Class->GetName();
		if (!Class->IsChildOf<ASCPBaseItem>())
		{
			continue;
		}

		if (Class->HasAnyClassFlags(CLASS_Deprecated | CLASS_Abstract))
		{
			continue;
		}

		// Ignore built in classes and our own test classes
		if (ClassName.StartsWith("SKEL_") || ClassName.StartsWith("REINST") || ClassName.EndsWith("Test"))
		{
			continue;
		}

		Classes.Add(Class);
	}

	return Classes;
}
