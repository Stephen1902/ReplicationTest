// Copyright 2025 DME Games

#include "ReppedCampfire.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AReppedCampfire::AReppedCampfire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Comp"));
	CollisionComp->SetSphereRadius(56.f);
	CollisionComp->SetupAttachment(RootComp);
	CollisionComp->SetIsReplicated(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetIsReplicated(true);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));
	ParticleSystemComp->SetupAttachment(MeshComp);
	
	bIsLit = false;
}
/*
void AReppedCampfire::ChangeLitMode_Implementation()
{

}
*/
// Called when the game starts or when spawned
void AReppedCampfire::BeginPlay()
{
	Super::BeginPlay();

	// We only want this function to be called by the server
	if (HasAuthority())
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AReppedCampfire::OnComponentHit);
	}
}

void AReppedCampfire::OnComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsLit = !bIsLit;
//	ChangeLitMode();
	// The function is called automatically on the clients, but not on the server.  Call it manually.
	OnRep_ChangeLitMode();
}
/*
// Called every frame
void AReppedCampfire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/
void AReppedCampfire::OnRep_ChangeLitMode() const
{
	if (ParticleSystemComp)
	{
		if (bIsLit)
		{
			ParticleSystemComp->Activate();
		}
		else
		{
			ParticleSystemComp->Deactivate();
		}
	}
}


void AReppedCampfire::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AReppedCampfire, bIsLit);
}
