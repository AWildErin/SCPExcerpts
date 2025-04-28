/**
 * Tests follow the format of (Generator).(Seed), unless specified otherwise
 *
 * @todo Maybe make these specs instead? So we can have different tests for each seed, such as required rooms etc.
 * @todo maybe move these into game? I don't think we should but that seems like a common place for them
 * @note SCP:CB's map grid is MapWidth/Height +1.
 */

#include "SCPCBGeneratorTests.h"

#if WITH_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "JsonUtilities.h"

#include "Map/Generators/SCPCBGenerator.h"
#include "SCPEditorModule.h"

constexpr int MAP_WIDTH = 18;
constexpr int MAP_HEIGHT = 18;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCPCBGenerator_MyMap, "SCP.Map.CBGenerator.MyMap", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCPCBGenerator_DONTBLINK, "SCP.Map.CBGenerator.DONTBLINK", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCPCBGenerator_JORGE, "SCP.Map.CBGenerator.JORGE", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCPCBGenerator_d9341, "SCP.Map.CBGenerator.d9341", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCPCBGenerator_dirtymetal, "SCP.Map.CBGenerator.dirtymetal", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCPCBGenerator_MyMap::RunTest(const FString&)
{
	TObjectPtr<USCPCBGenerator> Generator = NewObject<USCPCBGenerator>();
	Generator->InitializeGenerator(nullptr, MAP_WIDTH, MAP_HEIGHT);
	Generator->GenerateLevel("MyMap");

	FString FilePath = FPaths::ProjectDir() / "Tests/Data/CBGenerator_MyMap.json";
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *FilePath);

	TArray<FSCPCBGenTestDataEntry> TestData;
	bool bRet = FJsonObjectConverter::JsonArrayStringToUStruct(JsonString, &TestData);
	TestTrue("Failed to read json data!", bRet);

	auto& MapArray = Generator->GetMapArray();
	for (int X = 0; X < MAP_WIDTH+1; X++)
	{
		for (int Y = 0; Y < MAP_HEIGHT+1; Y++)
		{
			int Index = ((MAP_WIDTH + 1) * X) + Y;
			FSCPCBGenTestDataEntry TestEntry = TestData[Index];

			auto& MapEntry = MapArray[X][Y];

			TestEqual("PosX", MapEntry.PosX, TestEntry.PosX);
			TestEqual("PosY", MapEntry.PosY, TestEntry.PosY);
			TestEqual(FString::Printf(TEXT("(%d, %d) GridType"), X, Y), MapEntry.GridType, TestEntry.GridType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomType"), X, Y), (int)MapEntry.RoomType, TestEntry.RoomType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomZone"), X, Y), (int)MapEntry.RoomZone, TestEntry.RoomZone);
		}
	}

	return true;
}

bool FSCPCBGenerator_DONTBLINK::RunTest(const FString&)
{
	TObjectPtr<USCPCBGenerator> Generator = NewObject<USCPCBGenerator>();
	Generator->InitializeGenerator(nullptr, MAP_WIDTH, MAP_HEIGHT);
	Generator->GenerateLevel("DONTBLINK");

	FString FilePath = FPaths::ProjectDir() / "Tests/Data/CBGenerator_DONTBLINK.json";
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *FilePath);

	TArray<FSCPCBGenTestDataEntry> TestData;
	bool bRet = FJsonObjectConverter::JsonArrayStringToUStruct(JsonString, &TestData);
	TestTrue("Failed to read json data!", bRet);

	auto& MapArray = Generator->GetMapArray();
	for (int X = 0; X < MAP_WIDTH + 1; X++)
	{
		for (int Y = 0; Y < MAP_HEIGHT + 1; Y++)
		{
			int Index = ((MAP_WIDTH + 1) * X) + Y;
			FSCPCBGenTestDataEntry TestEntry = TestData[Index];

			auto& MapEntry = MapArray[X][Y];

			TestEqual("PosX", MapEntry.PosX, TestEntry.PosX);
			TestEqual("PosY", MapEntry.PosY, TestEntry.PosY);
			TestEqual(FString::Printf(TEXT("(%d, %d) GridType"), X, Y), MapEntry.GridType, TestEntry.GridType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomType"), X, Y), (int)MapEntry.RoomType, TestEntry.RoomType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomZone"), X, Y), (int)MapEntry.RoomZone, TestEntry.RoomZone);
		}
	}

	return true;
}


bool FSCPCBGenerator_JORGE::RunTest(const FString&)
{
	TObjectPtr<USCPCBGenerator> Generator = NewObject<USCPCBGenerator>();
	Generator->InitializeGenerator(nullptr, MAP_WIDTH, MAP_HEIGHT);
	Generator->GenerateLevel("JORGE");

	FString FilePath = FPaths::ProjectDir() / "Tests/Data/CBGenerator_JORGE.json";
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *FilePath);

	TArray<FSCPCBGenTestDataEntry> TestData;
	bool bRet = FJsonObjectConverter::JsonArrayStringToUStruct(JsonString, &TestData);
	TestTrue("Failed to read json data!", bRet);

	auto& MapArray = Generator->GetMapArray();
	for (int X = 0; X < MAP_WIDTH + 1; X++)
	{
		for (int Y = 0; Y < MAP_HEIGHT + 1; Y++)
		{
			int Index = ((MAP_WIDTH + 1) * X) + Y;
			FSCPCBGenTestDataEntry TestEntry = TestData[Index];

			auto& MapEntry = MapArray[X][Y];

			TestEqual("PosX", MapEntry.PosX, TestEntry.PosX);
			TestEqual("PosY", MapEntry.PosY, TestEntry.PosY);
			TestEqual(FString::Printf(TEXT("(%d, %d) GridType"), X, Y), MapEntry.GridType, TestEntry.GridType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomType"), X, Y), (int)MapEntry.RoomType, TestEntry.RoomType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomZone"), X, Y), (int)MapEntry.RoomZone, TestEntry.RoomZone);
		}
	}

	return true;
}


bool FSCPCBGenerator_d9341::RunTest(const FString&)
{
	TObjectPtr<USCPCBGenerator> Generator = NewObject<USCPCBGenerator>();
	Generator->InitializeGenerator(nullptr, MAP_WIDTH, MAP_HEIGHT);
	Generator->GenerateLevel("d9341");

	FString FilePath = FPaths::ProjectDir() / "Tests/Data/CBGenerator_d9341.json";
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *FilePath);

	TArray<FSCPCBGenTestDataEntry> TestData;
	bool bRet = FJsonObjectConverter::JsonArrayStringToUStruct(JsonString, &TestData);
	TestTrue("Failed to read json data!", bRet);

	auto& MapArray = Generator->GetMapArray();
	for (int X = 0; X < MAP_WIDTH + 1; X++)
	{
		for (int Y = 0; Y < MAP_HEIGHT + 1; Y++)
		{
			int Index = ((MAP_WIDTH + 1) * X) + Y;
			FSCPCBGenTestDataEntry TestEntry = TestData[Index];

			auto& MapEntry = MapArray[X][Y];

			TestEqual("PosX", MapEntry.PosX, TestEntry.PosX);
			TestEqual("PosY", MapEntry.PosY, TestEntry.PosY);
			TestEqual(FString::Printf(TEXT("(%d, %d) GridType"), X, Y), MapEntry.GridType, TestEntry.GridType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomType"), X, Y), (int)MapEntry.RoomType, TestEntry.RoomType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomZone"), X, Y), (int)MapEntry.RoomZone, TestEntry.RoomZone);
		}
	}

	return true;
}


bool FSCPCBGenerator_dirtymetal::RunTest(const FString&)
{
	TObjectPtr<USCPCBGenerator> Generator = NewObject<USCPCBGenerator>();
	Generator->InitializeGenerator(nullptr, MAP_WIDTH, MAP_HEIGHT);
	Generator->GenerateLevel("dirtymetal");

	FString FilePath = FPaths::ProjectDir() / "Tests/Data/CBGenerator_dirtymetal.json";
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *FilePath);

	TArray<FSCPCBGenTestDataEntry> TestData;
	bool bRet = FJsonObjectConverter::JsonArrayStringToUStruct(JsonString, &TestData);
	TestTrue("Failed to read json data!", bRet);

	auto& MapArray = Generator->GetMapArray();
	for (int X = 0; X < MAP_WIDTH + 1; X++)
	{
		for (int Y = 0; Y < MAP_HEIGHT + 1; Y++)
		{
			int Index = ((MAP_WIDTH + 1) * X) + Y;
			FSCPCBGenTestDataEntry TestEntry = TestData[Index];

			auto& MapEntry = MapArray[X][Y];

			TestEqual("PosX", MapEntry.PosX, TestEntry.PosX);
			TestEqual("PosY", MapEntry.PosY, TestEntry.PosY);
			TestEqual(FString::Printf(TEXT("(%d, %d) GridType"), X, Y), MapEntry.GridType, TestEntry.GridType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomType"), X, Y), (int)MapEntry.RoomType, TestEntry.RoomType);
			TestEqual(FString::Printf(TEXT("(%d, %d) RoomZone"), X, Y), (int)MapEntry.RoomZone, TestEntry.RoomZone);
		}
	}

	return true;
}

#endif
