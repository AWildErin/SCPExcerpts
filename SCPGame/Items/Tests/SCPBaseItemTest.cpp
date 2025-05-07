#include "Items/Tests/SCPBaseItemTest.h"

#include "SCPGameModule.h"

ASCPBaseItemTest::ASCPBaseItemTest()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASCPBaseItemTest::BeginPlay()
{
	Super::BeginPlay();
}

void ASCPBaseItemTest::OnItemUseSuccess_Implementation(AActor* User)
{
	UE_LOG(LogSCPItem, Log, TEXT("Successful use on %s by %s!"), *GetNameSafe(this), *GetNameSafe(User));
}

bool ASCPBaseItemTest::OnItemUseBegin_Implementation(AActor* User)
{
	UE_LOG(LogSCPItem, Log, TEXT("Attempted use on %s by %s!"), *GetNameSafe(this), *GetNameSafe(User));

	// Always succeed
	return true;
}
