
#if WITH_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/AutomationEditorCommon.h"

#include "Components/SCPInventoryComponent.h"
#include "Items/Tests/SCPBaseItemTest.h"
#include "SCPEditorModule.h"
#include "Tests/Common/SCPAutomationActorTest.h"

/** Tests picking up an item into the first slot */
IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FInventoryPickupTest, FSCPAutomationActorTest, "SCP.Inventory.Pickup",
										EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

/** Tests picking up an item into a specific slot */
IMPLEMENT_CUSTOM_SIMPLE_AUTOMATION_TEST(FInventoryPickupSlottedTest, FSCPAutomationActorTest, "SCP.Inventory.PickupSlotted",
										EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

/**
 * Some notes about our tests
 * - We have a specific test item actor, this is because the base item is abstract and we cannot spawn abstract classes.
 *
 * - We use do a cast when attempting to retrieve the actor from the inventory. This is because we're doing a direct
 * TestEqual, which doesn't like the fact we're passing an object. We could do the test on the result but this is fine.
 */

bool FInventoryPickupTest::RunTest(const FString& Parameters)
{
	if (!InitializeTest())
	{
		return false;
	}

	UWorld* World = GetTestWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the interactor and inventory
	AActor* Actor = World->SpawnActor<AActor>(SpawnParams);
	USCPInventoryComponent* Inventory = NewObject<USCPInventoryComponent>(Actor);
	Inventory->InventorySize = 4;
	Inventory->RegisterComponent();

	// Verify the inventory was created correctly
	TestEqual("Inventory size", Inventory->GetInventoryArray().Num(), 4);

	// Spawn the item
	ASCPBaseItemTest* Item = World->SpawnActor<ASCPBaseItemTest>(SpawnParams);

	// Add the item to the inventory
	TestTrue("Add item to inventory", Inventory->AddItem(Item));

	// It should be added to the 0 index
	TestEqual("Item is in index 0", Cast<ASCPBaseItemTest>(Inventory->GetItemInSlot(0)), Item);

	TearDownTest();

	return true;
}

bool FInventoryPickupSlottedTest::RunTest(const FString& Parameters)
{
	if (!InitializeTest())
	{
		return false;
	}

	UWorld* World = GetTestWorld();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the interactor and inventory
	AActor* Actor = World->SpawnActor<AActor>(SpawnParams);
	USCPInventoryComponent* Inventory = NewObject<USCPInventoryComponent>(Actor);
	Inventory->InventorySize = 4;
	Inventory->RegisterComponent();

	// Verify the inventory was created correctly
	TestEqual("Inventory size", Inventory->GetInventoryArray().Num(), 4);

	// Spawn the item
	ASCPBaseItemTest* Item = World->SpawnActor<ASCPBaseItemTest>(SpawnParams);

	// Add the item to the inventory
	TestTrue("Add item to inventory", Inventory->AddItem(Item, 2));

	// It should be added to the 2nd index
	TestEqual("Item is in index 2", Cast<ASCPBaseItemTest>(Inventory->GetItemInSlot(2)), Item);

	TearDownTest();

	return true;
}

#endif
