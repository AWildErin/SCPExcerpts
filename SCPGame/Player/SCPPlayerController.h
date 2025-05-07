#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SCPPlayerController.generated.h"

UCLASS()
class SCPGAME_API ASCPPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASCPPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
