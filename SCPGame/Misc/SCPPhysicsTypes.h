#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "SCPPhysicsTypes.generated.h"

namespace FSCPCollisionTypes
{
static constexpr ECollisionChannel ECC_Interactable = ECC_GameTraceChannel1;
static constexpr ECollisionChannel ECC_Item = ECC_GameTraceChannel2;
};

namespace FSCPCollisionPresets
{
static const FName Preset_Interactable = FName(TEXT("Interactable"));
};

/**
 * @enum ESCPPhysicalSurface
 * Keep in sync with the project settings
 */
UENUM(BlueprintType)
enum class ESCPPhysicalSurface : uint8
{
	None = 0,
	Default			= EPhysicalSurface::SurfaceType_Default,
	Metal			= EPhysicalSurface::SurfaceType1,
	Forest			= EPhysicalSurface::SurfaceType2,
	PocketDimension	= EPhysicalSurface::SurfaceType2,

};

FORCEINLINE bool operator==(const EPhysicalSurface& Surf, const ESCPPhysicalSurface& Surf2)
{
	return static_cast<int32>(Surf) == static_cast<int32>(Surf2);
};

FORCEINLINE bool operator!=(const EPhysicalSurface& Surf, const ESCPPhysicalSurface& Surf2)
{
	return !(Surf == Surf2);
};
