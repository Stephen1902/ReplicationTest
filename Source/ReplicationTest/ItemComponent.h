// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPLICATIONTEST_API UItemComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

	FDataTableRowHandle GetDataTableRowHandle() const { return DataTableRowHandle; }

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FDataTableRowHandle DataTableRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 Quantity;

private:
	virtual void OnActorInteracted_Implementation(AReplicationTestCharacter* PlayerWhoInteracted) override;
};
