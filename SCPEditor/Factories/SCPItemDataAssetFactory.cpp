#include "Factories/SCPItemDataAssetFactory.h"

#include "Data/SCPItemDataAsset.h"

USCPItemDataAssetFactory::USCPItemDataAssetFactory()
{
	SupportedClass = USCPItemDataAsset::StaticClass();
	bEditAfterNew = true;
	bCreateNew = true;
}

UObject* USCPItemDataAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(InClass->IsChildOf(USCPItemDataAsset::StaticClass()));
	return NewObject<USCPItemDataAsset>(InParent, InClass, InName, Flags);
}
