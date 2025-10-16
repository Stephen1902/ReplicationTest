// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "ChestActor.generated.h"

UCLASS()
class REPLICATIONTEST_API AChestActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChestActor();

	TSubclassOf<UUserWidget> GetWidgetToDisplay() const { return WidgetToDisplay; }

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Chest")
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Chest")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Chest")
	class UInventoryComp* InventoryComp;

	// Allows for chests and other items to have different widgets to use
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Chest")
	TSubclassOf<UUserWidget> WidgetToDisplay;

private:
	virtual void OnActorInteracted_Implementation(AReplicationTestCharacter* PlayerWhoInteracted) override;;
};
