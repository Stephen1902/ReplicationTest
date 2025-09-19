// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "PickupActorBase.generated.h"

UCLASS()
class REPLICATIONTEST_API APickupActorBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Items")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Items")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Items")
	class UItemComponent* ItemComponent;
	
	// Text to send when a player's line trace hits this to confirm it is interactive
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Items")
	FText TextToDisplay;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual FText OnActorViewed_Implementation() override; 
};
