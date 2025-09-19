// Copyright 2025 DME Games

#include "PickupActorBase.h"
#include "ItemComponent.h"
#include "WorldObjects.h"

// Sets default values
APickupActorBase::APickupActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	MeshComp->SetIsReplicated(true);

	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("Item Component"));

	TextToDisplay = FText::FromString("Default Text");
}

// Called when the game starts or when spawned
void APickupActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FText APickupActorBase::OnActorViewed_Implementation()
{
	//IInteractInterface::OnActorViewed();

	FString TextToReturn = "Row Handle Find Failed.";

	FDataTableRowHandle RowHandle = ItemComponent->GetDataTableRowHandle();
	if (!RowHandle.IsNull())
	{
		FWorldItemStruct* ItemStruct = RowHandle.DataTable->FindRow<FWorldItemStruct>(RowHandle.RowName, "");
		TextToReturn = "Pick up " + ItemStruct->ItemName.ToString();
	}
	
	return FText::FromString(TextToReturn);
}

