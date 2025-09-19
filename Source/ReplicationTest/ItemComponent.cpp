// Copyright 2025 DME Games


#include "ItemComponent.h"
#include "InventoryComp.h"
#include "ReplicationTestCharacter.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UDataTable> ItemDT(TEXT("/Game/WorldItems/DT_WorldItems"));
	if (ItemDT.Succeeded())
	{
		DataTableRowHandle.DataTable = ItemDT.Object;
	}

	Quantity = 1;
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->GetIsReplicated())
	{
		GetOwner()->SetReplicates(true);
	}

	SetIsReplicated(true);
}

// This is dealt with here and not the inventory component as this component only exists on picked up items
void UItemComponent::OnActorInteracted_Implementation(AReplicationTestCharacter* PlayerWhoInteracted)
{
	IInteractInterface::OnActorInteracted_Implementation(PlayerWhoInteracted);

	// Check the player is valid and they have a valid inventory component
	if (PlayerWhoInteracted && PlayerWhoInteracted->GetInventoryComp())
	{
		// Check that the player has space for the item this is
		if (PlayerWhoInteracted->GetInventoryComp()->AddToInventory(DataTableRowHandle.RowName, Quantity))
		{
			GetOwner()->Destroy();
		}
	}
	
}

