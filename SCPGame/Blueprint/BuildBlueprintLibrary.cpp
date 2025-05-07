#include "Blueprint/BuildBlueprintLibrary.h"

#include "VersionInfo.h"
#include "Misc/DateTime.h"

FString UBuildBlueprintLibrary::GetBuildBranch()
{
	return FVersionInfo::BranchName;
}

FString UBuildBlueprintLibrary::GetBuildCommitShort()
{
	return FVersionInfo::CommitShort;
}

FString UBuildBlueprintLibrary::GetBuildCommitLong()
{
	return FVersionInfo::CommitLong;
}

FString UBuildBlueprintLibrary::GetBuildDateUTC()
{
	return FVersionInfo::BuildTimeUTC;
}

FString UBuildBlueprintLibrary::GetProjectVersion()
{
	FString Version;
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), Version, GGameIni);
	return Version;
}

bool UBuildBlueprintLibrary::IsShippingBuild()
{
	return UE_BUILD_SHIPPING;
}

bool UBuildBlueprintLibrary::IsDevelopmentBuild()
{
	return UE_BUILD_DEVELOPMENT;
}

bool UBuildBlueprintLibrary::IsDebugBuild()
{
	return UE_BUILD_DEBUG;
}

bool UBuildBlueprintLibrary::IsTestBuild()
{
	return UE_BUILD_TEST;
}

bool UBuildBlueprintLibrary::IsWithEditor()
{
	return WITH_EDITOR;
}

