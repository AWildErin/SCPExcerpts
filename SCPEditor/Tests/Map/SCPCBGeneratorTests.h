#pragma once

#include "CoreMinimal.h"

#include "SCPCBGeneratorTests.generated.h"


USTRUCT()
struct SCPEDITOR_API FSCPCBGenTestDataEntry
{
	GENERATED_BODY();

	UPROPERTY()
	FString RoomName = "";

	UPROPERTY()
	int PosX = 0;

	UPROPERTY()
	int PosY = 0;

	UPROPERTY()
	int GridType = 0;

	UPROPERTY()
	int RoomType = 0;

	UPROPERTY()
	int RoomZone = 0;
};
