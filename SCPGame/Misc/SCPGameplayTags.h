#pragma once

#include "NativeGameplayTags.h"

namespace SCPGameplayTags
{
	SCPGAME_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Movement
	SCPGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	SCPGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	SCPGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	SCPGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	SCPGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Sprint);

	// Actions
	SCPGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Blink);
	SCPGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact);
	SCPGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Inventory);

	// Menus
	SCPGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Pause);

}; // namespace SCPGameplayTags
