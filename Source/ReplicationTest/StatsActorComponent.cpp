// Copyright 2024 DME Games

#include "StatsActorComponent.h"
#include "ReplicationTestCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UStatsActorComponent::UStatsActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	StartingHunger = 5.f;
	MaxHunger = 100.f;

	Hunger = StartingHunger;

	FoodDropPerSecond = -1.0f;

	// Amount of gap between timers being fired.  0.05f is 20 times per second
	FoodTimerFrequency = 0.1f;

	PlayerCharacter = nullptr;
}


// Called when the game starts
void UStatsActorComponent::BeginPlay()
{
	Super::BeginPlay();

	SetIsReplicated(true);
	if (!GetOwner()->GetIsReplicated())
	{
		GetOwner()->SetReplicates(true);
	}

	PlayerCharacter = Cast<AReplicationTestCharacter>(GetOwner());

	GetWorld()->GetTimerManager().SetTimer(FoodTimerHandle, this, &UStatsActorComponent::FoodDrain, FoodTimerFrequency, true, FoodTimerFrequency);
}

void UStatsActorComponent::OnRep_Hunger()
{
	UE_LOG(LogTemp, Warning, TEXT("%s has hunger of %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(Hunger));
}

void UStatsActorComponent::FoodDrain()
{
	UpdateValues(FoodDropPerSecond * FoodTimerFrequency, 0.f, 0.f);
}

void UStatsActorComponent::UpdateValues(float FoodToAdjust, float WaterToAdjust, float HealthToAdjust)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_UpdateValues(FoodToAdjust, WaterToAdjust, HealthToAdjust);
		return;
	}

	Hunger = FMath::Clamp(Hunger + FoodToAdjust, 0.f, MaxHunger);

	// OnRep functions don't get called on the server automatically, call it now.
	OnRep_Hunger();
}

void UStatsActorComponent::Server_UpdateValues_Implementation(float FoodToAdjust, float WaterToAdjust, float HealthToAdjust)
{
	UpdateValues(FoodToAdjust, WaterToAdjust, HealthToAdjust);
}


void UStatsActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStatsActorComponent, Hunger);
}
