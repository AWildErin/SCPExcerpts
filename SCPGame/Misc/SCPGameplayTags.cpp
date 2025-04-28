#include "SCPGameplayTags.h"

namespace SCPGameplayTags
{
	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		return FGameplayTag();
	}

	// Movement
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move,		"InputTag.Move",		"Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse",	"Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick",	"Look (stick) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch,		"InputTag.Crouch",		"Crouch input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Sprint,		"InputTag.Sprint",		"Sprint input.");

	// Actions
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Blink,		"InputTag.Blink",		"Blink input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Interact,	"InputTag.Interact",	"Interact input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Inventory,	"InputTag.Inventory",	"Inventory input.");

	// Menus
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Pause,		"InputTag.Pause",		"Pause input.");

} // namespace SCPGameplayTags
