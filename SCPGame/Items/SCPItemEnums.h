#pragma once

#include "CoreMinimal.h"

/**
 * @enum ESCPItemSlot
 * Item slots for the player
 */
UENUM(BlueprintType)
enum ESCPItemSlot : uint8
{
	SlotNone,

	/** Items in our hand, such as keycards, s-nav or radio*/
	Hand = 1,

	/** Items on our head, such as gas masks or nvgs */
	Head = 2,

	/** Items on our budy, such as hazmat suit or ballistic vest */
	Body = 3,

	Ring = 4,

	Neck = 5,
};

UENUM(BlueprintType)
enum ESCPKeycard : uint8
{
	LevelNone,

	LevelOne = 1,
	LevelTwo = 2,
	LevelThree = 3,
	LevelFour = 4,
	LevelFive = 5,

	Omni = 6,
};
