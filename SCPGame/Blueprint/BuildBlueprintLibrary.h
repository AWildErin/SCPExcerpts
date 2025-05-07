#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BuildBlueprintLibrary.generated.h"

UCLASS()
class UBuildBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** @returns the git branch the code was built from */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static FString GetBuildBranch();

	/** @returns the git commit the code was built from */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static FString GetBuildCommitShort();

	/** @returns the git commit the code was built from */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static FString GetBuildCommitLong();

	/** @returns a UTC date when the project was built */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static FString GetBuildDateUTC();

	/** @returns the current version of the project from the config */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static FString GetProjectVersion();

	/**
	 * @returns whether or not we were built in shipping
	 * @note Do not use to disable code in C++. Use the define directly using UE_BUILD_SHIPPING
	 */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static bool IsShippingBuild();

	/**
	 * @returns whether or not we were built in development
	 * @note Do not use to disable code in C++. Use the define directly using UE_BUILD_DEVELOPMENT
	 */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static bool IsDevelopmentBuild();

	/**
	 * @returns whether or not we were built in debug
	 * @note Do not use to disable code in C++. Use the define directly using UE_BUILD_DEBUG
	 */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static bool IsDebugBuild();

	/**
	 * @returns whether or not we were built in test
	 * @note Do not use to disable code in C++. Use the define directly using UE_BUILD_TEST
	 */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static bool IsTestBuild();

	/**
	 * @returns whether or not we were built with the editor enabled.
	 * @note Do not use to disable code in C++. Use the define directly using WITH_EDITIOR
	 */
	UFUNCTION(BlueprintPure, Category = "BuildInfo")
	static bool IsWithEditor();
};
