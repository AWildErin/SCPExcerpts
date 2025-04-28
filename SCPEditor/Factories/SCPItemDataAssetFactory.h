#pragma once

#include "CoreMinimal.h"

#include "Factories/Factory.h"

#include "SCPItemDataAssetFactory.generated.h"

UCLASS()
class USCPItemDataAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	USCPItemDataAssetFactory();

	// Begin UFactory Interface
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End UFactory Interface
};
