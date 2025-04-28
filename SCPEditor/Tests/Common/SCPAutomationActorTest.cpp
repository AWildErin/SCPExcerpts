#include "Tests/Common/SCPAutomationActorTest.h"

#include "Tests/AutomationCommon.h"
#include "Tests/AutomationEditorCommon.h"

#include "Tests/Common/SCPAutomationCommon.h"

#if WITH_AUTOMATION_TESTS

bool FSCPAutomationActorTest::InitializeTest(const FString& WorldPath)
{
	ensureMsgf(!bInitialized, TEXT("Test was initialized more than once!"));

	// Ignore getDriverInfo error. It's fine, fmod would handle this itself
	// but Unreal will complain because it's technically an error while running the test
	AddExpectedError("'lowLevelSystem->getDriverInfo\\(.*\\)' returned '.*'");

	// We can't do FAutomationEditorCommonUtils::CreateNewMap() for some reason, so we're forced to make an empty level ourselves
	// if we want a completely blank world using AGameModeBase
	/** @todo Is there any way we could speed this up? */
	if (!AutomationOpenMap(WorldPath))
	{
		TearDownTest();
	
		AddError("Failed to open map");
		return false;
	}

	TestWorld = FSCPAutomationCommon::GetAnyGameWorld();

	if (!TestWorld)
	{
		TearDownTest();

		AddError("Failed to get game world");
		return false;
	}

	bInitialized = true;

	return true;
}

void FSCPAutomationActorTest::TearDownTest()
{
	bInitialized = false;
	TestWorld = nullptr;

	ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand());
}

UWorld* FSCPAutomationActorTest::GetTestWorld()
{
	checkf(bInitialized, TEXT("Please make sure test is initialized!"));
	return TestWorld;
}

#endif
