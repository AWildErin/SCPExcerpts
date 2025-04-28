#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SCPRoomDataAssetFactory.generated.h"

UCLASS()
class USCPRoomDataAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	USCPRoomDataAssetFactory();

	// Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End UFactory Interface
};
