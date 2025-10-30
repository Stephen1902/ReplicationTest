// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsActorComponent.generated.h"

class AReplicationTestCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPLICATIONTEST_API UStatsActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float StartingHunger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxHunger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float FoodDropPerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float FoodTimerFrequency;

private:
	UPROPERTY(ReplicatedUsing=OnRep_Hunger)
	float Hunger;

	UFUNCTION()
	void OnRep_Hunger();

	void FoodDrain();

	UFUNCTION(Server, Reliable)
	void Server_UpdateValues(float FoodToAdjust, float WaterToAdjust, float HealthToAdjust);
	
	// Timer Handles
	FTimerHandle FoodTimerHandle;

	UPROPERTY()
	AReplicationTestCharacter* PlayerCharacter;

public:
	void UpdateValues(float FoodToAdjust, float WaterToAdjust, float HealthToAdjust);
};
