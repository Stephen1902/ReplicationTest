// Copyright 2025 DME Games


#include "ChestActor.h"
#include "InventoryComp.h"
#include "PlayerWidget.h"
#include "RepPlayerController.h"

// Sets default values
AChestActor::AChestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComp);

	InventoryComp = CreateDefaultSubobject<UInventoryComp>(TEXT("Inventory Comp"));
	InventoryComp->SetArraySize(8);
}

// Called when the game starts or when spawned
void AChestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChestActor::OnActorInteracted_Implementation(AReplicationTestCharacter* PlayerWhoInteracted)
{
	IInteractInterface::OnActorInteracted_Implementation(PlayerWhoInteracted);

	if (PlayerWhoInteracted)
	{
		if (ARepPlayerController* PC = Cast<ARepPlayerController>(PlayerWhoInteracted->GetController()))
		{
			// The owner needs to be set to something in the world that can "use it".  Set the interacting player as the owner.
			SetOwner(PlayerWhoInteracted);
			PC->ShowContainer(WidgetToDisplay, InventoryComp, PlayerWhoInteracted);
		}
	}
}
