#include "Factories/SCPRoomDataAssetFactory.h"

#include "Map/SCPRoomDataAsset.h"

USCPRoomDataAssetFactory::USCPRoomDataAssetFactory()
{
	SupportedClass = USCPRoomDataAsset::StaticClass();
	bEditAfterNew = true;
	bCreateNew = true;
}

UObject* USCPRoomDataAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(USCPRoomDataAsset::StaticClass()));
	return NewObject<USCPRoomDataAsset>(InParent, InClass, InName, Flags);
}
