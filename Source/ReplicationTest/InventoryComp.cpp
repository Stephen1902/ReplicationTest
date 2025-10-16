// Copyright 2025 DME Games

#include "InventoryComp.h"
#include "ItemComponent.h"
#include "ReplicationTestCharacter.h"
#include "WorldObjects.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComp::UInventoryComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UDataTable> ItemDT(TEXT("/Game/WorldItems/DT_WorldItems"));
	if (ItemDT.Succeeded())
	{
		DataTableRowHandle.DataTable = ItemDT.Object;
	}

	InventorySize = 16;
	TraceDistance = 500.f;
	ActorBeenHit = nullptr;
}

// Called when the game starts
void UInventoryComp::BeginPlay()
{
	Super::BeginPlay();

	// Needed to ensure component works with multiplayer.  As this is a component, owner also needs to replicate.
	if (!GetOwner()->GetIsReplicated())
	{
		GetOwner()->SetReplicates(true);
	}
	SetIsReplicated(true);

	
	// Resize the array full of empty spaces
	InventoryContent.SetNum(InventorySize);
	/*for (int32 i = 0; i < InventoryContent.Num(); ++i)
	{
		InventoryContent[i].Quantity = 0;
	}*/
}

void UInventoryComp::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceForInteractive();
	
}

bool UInventoryComp::AddToInventory(FName ItemIDIn, int32 QuantityIn)
{
	// Needed to be used to ensure there is no infinite loop
	bool bHasFailed = false;
	
	// Things can only be added if there is a valid data table being used
	if (!DataTableRowHandle.IsNull())
	{
		int32 QuantityRemaining = QuantityIn;
		int32 SlotIndex = -1;

		do
		{
			if (FindSlot(ItemIDIn, SlotIndex))
			{
				AddToStack(SlotIndex, 1);
				QuantityRemaining -= 1;
			}
			else
			{
				if (HasEmptySlot(SlotIndex))
				{
					if (CreateNewStack(ItemIDIn, 1))
					{
						QuantityRemaining -= 1;
					}
					else
					{
						bHasFailed = true;
					}
				}
				else
				{
					bHasFailed = true;
				}
			}
		}
		while (QuantityRemaining > 0 && !bHasFailed);

		// Check that all items have been removed and the attempt didn't fail
		if (QuantityRemaining == 0 && !bHasFailed)
		{
			return true;
		}
	}

	return bHasFailed;
}

void UInventoryComp::RemoveFromInventory()
{
	
	

}

void UInventoryComp::SetArraySize(const int32 NumElements)
{
	InventorySize = NumElements;
}

void UInventoryComp::DealWithInteract(AReplicationTestCharacter* CharacterInteracting)
{
	if (ActorBeenHit && CharacterInteracting)
	{
		Server_DealWithInteract(ActorBeenHit, CharacterInteracting);
	}
}

void UInventoryComp::TraceForInteractive()
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	const FVector StartLoc = CameraManager->GetCameraLocation();
	const FVector EndLoc = (CameraManager->GetActorForwardVector() * TraceDistance) + StartLoc;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	FHitResult HitResult;

	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLoc, EndLoc, 16.f, UEngineTypes::ConvertToTraceType(COLLISION_INTERACTIVE), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true))
	{
		// Check that what is being his isn't the actor we already hit
		if (HitResult.GetActor() != ActorBeenHit)
		{
			ActorBeenHit = HitResult.GetActor();
			FText TextToDisplay = Execute_OnActorViewed(ActorBeenHit);
			GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TextToDisplay.ToString());
		}
	}
	else
	// Check that there was a hit actor, but there isn't one now
	{
		if (ActorBeenHit != nullptr)
		{
			Execute_OnActorViewLost(ActorBeenHit);
			ActorBeenHit = nullptr;
		}
	}
}

void UInventoryComp::UpdateInventory_Implementation()
{
	OnInventoryUpdated.Broadcast();
}

void UInventoryComp::Server_TransferSlot_Implementation(int32 SourceIndex, UInventoryComp* SourceInventory, int32 DestinationIndex)
{
	TransferSlots(SourceIndex, SourceInventory, DestinationIndex);
}

bool UInventoryComp::FindSlot(FName ItemID, int32& FoundSlot)
{
	for (int32 i = 0; i < InventoryContent.Num(); ++i)
	{
		// Check if there is an item matching this already in the inventory
		if (InventoryContent[i].ItemID == ItemID)
		{
			// Check if the slot has space for the item
			if (InventoryContent[i].Quantity < GetMaxStackSize(ItemID))
			{
				FoundSlot = i;
				return true;
			}
		}
	}
	
	FoundSlot = -1;
	return false;
}

int32 UInventoryComp::GetMaxStackSize(FName ItemID)
{
	// Check if the row exists in the data table
	if (const FWorldItemStruct* ItemStruct = DataTableRowHandle.DataTable->FindRow<FWorldItemStruct>(ItemID, ""))
	{
		return ItemStruct->ItemMaxStack;
	}
	
	return -1;
}

void UInventoryComp::AddToStack(const int32 SlotIndex, const int32 QuantityToAdd)
{
	InventoryContent[SlotIndex].Quantity += QuantityToAdd;
}

bool UInventoryComp::HasEmptySlot(int32& EmptySlot)
{
	for (int32 i = 0; i < InventoryContent.Num(); ++i)
	{
		if (InventoryContent[i].Quantity == 0)
		{
			EmptySlot = i;
			return true;
		}
	}

	EmptySlot = -1;
	return false;
}

bool UInventoryComp::CreateNewStack(FName ItemID, int32 Quantity)
{
	int32 SlotIndex = -1;
	if (HasEmptySlot(SlotIndex))
	{
		InventoryContent[SlotIndex].ItemID = ItemID;
		InventoryContent[SlotIndex].Quantity = Quantity;
		return true;
	}
	
	return false;
}

void UInventoryComp::Server_DealWithInteract_Implementation(AActor* ActorToInteractWith, AReplicationTestCharacter* CharacterInteracting)
{
	if (ActorToInteractWith && CharacterInteracting)
	{
		// Check if the actor has an ItemComponent.  If it does, it's for pick up.  Deal with this.
		if (UItemComponent* ItemComponent = Cast<UItemComponent>(ActorToInteractWith->GetComponentByClass(UItemComponent::StaticClass())))
		{
			Execute_OnActorInteracted(ItemComponent, CharacterInteracting);
		}
		else
		{
			// It doesn't have an item component so is probably a chest.

			// Only an owned object can be used with the inventory system.  Set the owner. 
			ActorToInteractWith->SetOwner(CharacterInteracting->GetController());

			// Only one player at a time should be able to interact.  Call the client only function.
			OnLocalInteract(ActorToInteractWith, CharacterInteracting);
		}
	}
}


void UInventoryComp::TransferSlots(int32 SourceIndex, UInventoryComp* SourceInventory, int32 DestinationIndex)
{
	if (SourceInventory)
	{
		TArray<FItemStruct> SInventoryContents = SourceInventory->GetInventory();
		FItemStruct CurrentItem = SInventoryContents[SourceIndex];

		// Check if the destination index is valid.
		if (DestinationIndex < 0)
		{
			
		}
		else
		{
			// Check if the current item is the same as the destination item
			if (CurrentItem.ItemID == InventoryContent[DestinationIndex].ItemID)
			{
				
			}
			else
			{
				// They are different, swap them
				SourceInventory->SetItemArray(InventoryContent[DestinationIndex], SourceIndex);
				InventoryContent[DestinationIndex] = CurrentItem;
				// Update the inventory on screen
				UpdateInventory();
			}
		}
	}
}

void UInventoryComp::OnLocalInteract_Implementation(AActor* TargetActor, AReplicationTestCharacter* InteractingChar)
{
	Execute_OnActorInteracted(TargetActor, InteractingChar);
}


void UInventoryComp::SetItemArray(FItemStruct ItemStruct, int32 ItemIndex)
{
	InventoryContent[ItemIndex] = ItemStruct;
	// Update the inventory on screen
	UpdateInventory();
}


void UInventoryComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComp, InventoryContent);
}
