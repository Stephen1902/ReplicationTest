// Copyright 2025 DME Games

#include "PlantActor.h"

#include "ReplicationTestCharacter.h"
#include "WorldItemHealthWidget.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlantActor::APlantActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>("Collision Comp");
	CollisionComp->SetupAttachment(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Display Mesh");
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetIsReplicated(true);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget Comp");
	WidgetComponent->SetupAttachment(RootComp);
	
	TimeBetweenNewStage = 10.f;
	StartingHealth = 300.f;
}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APlantActor::OnComponentHit);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &APlantActor::OnComponentOverlapEnd);

	if (TimeBetweenNewStage > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(NextStageTimer, this, &APlantActor::NewStageTriggered_Implementation, TimeBetweenNewStage, true, TimeBetweenNewStage);
	}

	CurrentHealth = StartingHealth;
}

// Called every frame
void APlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlantActor::ChangeHealth(const float HealthChange)
{
	if (HealthChange != 0.f)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth + HealthChange, 0.f, StartingHealth);
		UE_LOG(LogTemp, Warning, TEXT("Health is now: %s"), *FString::SanitizeFloat(CurrentHealth));
		// Check if the health has dropped to 0, do as required if it has
		if (CurrentHealth <= 0.f)
		{
			Destroy();
		}
		else if (UWorldItemHealthWidget* HealthWidget = Cast<UWorldItemHealthWidget>(WidgetComponent->GetWidget()))
		{
			HealthWidget->UpdateHealthWidget(CurrentHealth, StartingHealth);
		}
	}
}

void APlantActor::OnComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AReplicationTestCharacter* TestCharacter = Cast<AReplicationTestCharacter>(OtherActor))
	{
		if (TestCharacter->IsLocallyControlled())
		{
			TestCharacter->SetPlantActor(this);
			WidgetComponent->SetVisibility(true);
		}
	}
}

void APlantActor::OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AReplicationTestCharacter* TestCharacter = Cast<AReplicationTestCharacter>(OtherActor))
	{
		if (TestCharacter->IsLocallyControlled())
		{
			TestCharacter->SetPlantActor(nullptr);
			WidgetComponent->SetVisibility(false);
		}
	}
}

void APlantActor::OnRepHealthChanged()
{
	if (UWorldItemHealthWidget* HealthWidget = Cast<UWorldItemHealthWidget>(WidgetComponent->GetWidget()))
	{
		HealthWidget->UpdateHealthWidget(CurrentHealth, StartingHealth);
	}
}

void APlantActor::NewStageTriggered_Implementation()
{
	
}

void APlantActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlantActor, CurrentHealth);
}
