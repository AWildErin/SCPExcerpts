#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

#if WITH_AUTOMATION_TESTS

/** Base class to scaffold a world for actor testing */
class FSCPAutomationActorTest : public FAutomationTestBase
{
public:
	FSCPAutomationActorTest(const FString& InName, const bool bInComplexTask) : FAutomationTestBase(InName, bInComplexTask)
	{
		TestWorld = nullptr;
		bInitialized = false;
	}

	/**
	* Scaffolds actor test level
	*
	* @param WorldPath Path to the world, EmptyTestLevel is a blank level with GameModeBase.
	*
	* @returns Whether or not we initialized correctly
	*/
	bool InitializeTest(const FString& WorldPath = "EmptyTestLevel");

	void TearDownTest();

	/** Returns the world for the given test */
	UWorld* GetTestWorld();

private:
	/** Has InitializeTest() been called? */
	bool bInitialized;

	/** World to house all the actors in */
	UWorld* TestWorld;
};

#endif
