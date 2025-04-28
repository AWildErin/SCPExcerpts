#include "Items/SCPBaseItem.h"

#include "Logging/StructuredLog.h"
#include "Components/SphereComponent.h"

#include "Components/SCPInventoryComponent.h"
#include "Misc/SCPPhysicsTypes.h"
#include "SCPGameModule.h"

ASCPBaseItem::ASCPBaseItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	SetRootComponent(ItemMesh);

	// We never collide with items, but we do trace against their physics
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionObjectType(FSCPCollisionTypes::ECC_Item);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ItemMesh->SetCollisionResponseToChannel(FSCPCollisionTypes::ECC_Item, ECollisionResponse::ECR_Overlap);
	ItemMesh->SetSimulatePhysics(true);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>("InteractionSphere");
	InteractionSphere->SetSphereRadius(64.f);
	InteractionSphere->SetCollisionProfileName(FSCPCollisionPresets::Preset_Interactable);
	InteractionSphere->SetupAttachment(RootComponent);

	bIsInInventory = false;
}

void ASCPBaseItem::BeginPlay()
{
	Super::BeginPlay();
}

void ASCPBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCPBaseItem::OnInteract_Implementation(AActor* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	TObjectPtr<USCPInventoryComponent> Inventory = Interactor->GetComponentByClass<USCPInventoryComponent>();
	if (!Inventory)
	{
		return;
	}

	if (!Inventory->AddItem(this))
	{
		return;
	}

	NativePickup(Interactor);
}

bool ASCPBaseItem::OnItemDroppedOnto_Implementation(ASCPBaseItem* OtherItem)
{
	return false;
}

bool ASCPBaseItem::OnItemUse(AActor* User)
{
	bool bReturn = OnItemUseBegin(User);
	if (bReturn)
	{
		OnItemUseSuccess(User);
	}
	else
	{
		OnItemUseFail(User);
	}

	return bReturn;
}

bool ASCPBaseItem::OnItemUseBegin_Implementation(AActor* User)
{
	return false;
}

void ASCPBaseItem::OnItemUseSuccess_Implementation(AActor* User) {}

void ASCPBaseItem::OnItemUseFail_Implementation(AActor* User) {}

void ASCPBaseItem::NativePickup(AActor* Interactor)
{
	SetOwner(Interactor);

	// Hide everything
	RootComponent->SetVisibility(false, true);

	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Attach to the owner
	if (!AttachToActor(Interactor, FAttachmentTransformRules::SnapToTargetNotIncludingScale))
	{
		UE_LOGFMT(LogSCPItem, Warning, "Failed to attach {Actor} to {Parent}", GetName(), Interactor->GetName());
	}

	OnItemPickedUp(Interactor);
}

void ASCPBaseItem::NativeDrop(AActor* Interactor)
{
	OnItemDropped(Interactor);

	SetOwner(nullptr);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetSimulatePhysics(true);

	/**
	 * @todo Verify this works in all our possible usecases even if we don't populate to children.
	 * populating to children causes issues where the meshes of objects in containers will also appear
	 */
	RootComponent->SetVisibility(true, false);
}
