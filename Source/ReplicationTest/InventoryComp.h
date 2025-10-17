// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "ReplicationTestCharacter.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "InventoryComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

#define COLLISION_INTERACTIVE	ECC_GameTraceChannel2

USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPLICATIONTEST_API UInventoryComp : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Default size for this inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component")
	int32 InventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component", Replicated)
	TArray<FItemStruct> InventoryContent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component")
	float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FDataTableRowHandle DataTableRowHandle;

public:
	bool AddToInventory(FName ItemIDIn, int32 QuantityIn);
	void RemoveFromInventory();

	void SetArraySize(const int32 NumElements);
	
	void DealWithInteract(AReplicationTestCharacter* CharacterInteracting);

	void SetItemArray(FItemStruct ItemStruct, int32 ItemIndex);
	TArray<FItemStruct> GetInventory() const { return InventoryContent; }

	UFUNCTION(NetMulticast, Reliable)
	void UpdateInventory();

	UPROPERTY()
	FOnInventoryUpdated OnInventoryUpdated;

	const UDataTable* GetDataTable() const { return DataTableRowHandle.DataTable; }

	UFUNCTION(Server, Reliable)
	void Server_TransferSlot(int32 SourceIndex, UInventoryComp* SourceInventory, int32 DestinationIndex);
private:
	void TraceForInteractive();

	UPROPERTY()
	AActor* ActorBeenHit;

	UFUNCTION(Server, Reliable)
	void Server_DealWithInteract(AActor* ActorToInteractWith, AReplicationTestCharacter* CharacterInteracting);

	bool FindSlot(FName ItemID, int32& FoundSlot);
	int32 GetMaxStackSize(FName ItemID);
	void AddToStack(const int32 SlotIndex, const int32 QuantityToAdd);
	bool HasEmptySlot(int32& EmptySlot);
	bool CreateNewStack(FName ItemID, int32 Quantity);

	void TransferSlots(int32 SourceIndex, UInventoryComp* SourceInventory, int32 DestinationIndex);

	UFUNCTION(Client, Reliable)
	void OnLocalInteract(AActor* TargetActor, AReplicationTestCharacter* InteractingChar);

	void PrepareInventoryUpdate();
};
