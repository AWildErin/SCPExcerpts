#pragma once

#include "CoreMinimal.h"

#if WITH_AUTOMATION_TESTS

namespace FSCPAutomationCommon
{

/** @note Mirrored from AutomationCommon.cpp due to the fact it might be removed in the future. */
UWorld* GetAnyGameWorld();

} // namespace FSCPAutomationCommon

#endif
