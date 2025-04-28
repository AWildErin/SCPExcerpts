#include "Player/SCPCheatManager.h"

#include "Logging/StructuredLog.h"
#include "EngineUtils.h"

#include "Player/SCPPlayerCharacter.h"
#include "Map/Generators/SCPCBGenerator.h"
#include "Map/SCPMapGenerator.h"
#include "SCPGameModule.h"

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
