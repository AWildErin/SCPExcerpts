#pragma once

#include "CoreMinimal.h"

#include "UI/SCPActivatableWidget.h"

#include "SCPDevMenuWidget.generated.h"

/**
* @struct FSCPReportInfo
* A struct that contains very basic level information
*
* @todo We should mirror this to crash reporting too when we set that up
* For above see https://dev.epicgames.com/documentation/en-us/unreal-engine/crash-reporting-in-unreal-engine#addcustomcontexttocrashreports
*/
USTRUCT()
struct FSCPReportInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int64 TimeStamp = 0;

	UPROPERTY()
	FString Description;

	UPROPERTY()
	FString LevelName;

	UPROPERTY()
	FString LevelSeed;

	UPROPERTY()
	FString BugItString;

	UPROPERTY()
	FString BuildCommit;

	UPROPERTY()
	FString BuildBranch;

	UPROPERTY()
	FString BuildTimeUTC;
};


UCLASS()
class USCPDevMenuWidget : public USCPActivatableWidget
{
	GENERATED_BODY()
public:
	USCPDevMenuWidget(const FObjectInitializer& ObjectInitializer);

	/** Generates a report zip and places it on the desktop. */
	UFUNCTION(BlueprintCallable, Category = "DevMenu")
	bool GenerateReport(FString ReportComment, FString& OutputPath);
};
