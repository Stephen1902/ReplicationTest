// Copyright 2025 DME Games

#include "WriteableChestActor.h"
#include "ReplicationTestCharacter.h"
#include "RepPlayerController.h"
#include "WriteableChestWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWriteableChestActor::AWriteableChestActor()
{
	bReplicates = true;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComponent->SetupAttachment(RootComp);

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render Comp 1"));
	TextRenderComponent->SetupAttachment(MeshComponent);
	TextRenderComponent->SetIsReplicated(true);
	
}

// Called when the game starts or when spawned
void AWriteableChestActor::BeginPlay()
{
	Super::BeginPlay();
		
}

// Called every frame
void AWriteableChestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWriteableChestActor::SetRenderText(const FText& TextIn)
{
	UE_LOG(LogTemp, Warning, TEXT("SetRenderText: TextIn is %s"), *TextIn.ToString())
 	if (!HasAuthority())
 	{
 		UE_LOG(LogTemp, Warning, TEXT("No authority"));
 		Server_SetRenderText(TextIn);
 	}
 	else
 	{
 		UE_LOG(LogTemp, Warning, TEXT("SetRenderText: HasAuthority."));
 		RenderText = TextIn;

 		// The OnRep function isn't called automatically on the server.  Call it manually.
 		OnRep_RenderTextUpdated();
 		UE_LOG(LogTemp, Warning, TEXT("RenderText is %s"), *RenderText.ToString());
 	}
/*
	if (HasAuthority())
	{
		RenderText = TextIn;
	
		OnRep_RenderTextUpdated();
	}
	*/
}

void AWriteableChestActor::WidgetHasBeenClosed()
{
	if (WriteableChestWidgetRef)
 	{
 		WriteableChestWidgetRef->RemoveFromParent();
 		WriteableChestWidgetRef = nullptr;
 	}

	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
		PC = nullptr;
	}
}

void AWriteableChestActor::OnRep_RenderTextUpdated()
{
	TextRenderComponent->SetText(RenderText);
}

FText AWriteableChestActor::OnActorViewed_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("You got me."));
	return FText::FromString("You got me.");
}

void AWriteableChestActor::OnActorViewLost_Implementation()
{

}

void AWriteableChestActor::OnActorInteracted_Implementation(AReplicationTestCharacter* CharacterInteracting)
{
	/*
	// Check a valid widget has been set and one is not already on screen
	if (CharacterIn && WidgetToDisplay)
	{
		PC = Cast<ARepPlayerController>(CharacterIn->GetController());
		if (PC)
		{
			PC->DealWithWriteableChest(this, WidgetToDisplay);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get Player Controller."));
		}
	}
	else
	{
		if (!CharacterIn)
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterIn is not valid."));
		}

		if (!WidgetToDisplay)
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterIn is not valid."));
		}
	}
	*/
}

void AWriteableChestActor::SetNewOwner(ARepPlayerController* ControllerIn)
{
	if (!HasAuthority())
	{
		Server_SetOwner(ControllerIn);
	}
	else
	{
		SetOwner(ControllerIn);
		UE_LOG(LogTemp, Warning, TEXT("Owner is %s"), *GetOwner()->GetName());
	}
}

void AWriteableChestActor::Server_SetOwner_Implementation(ARepPlayerController* ControllerIn)
{
	UE_LOG(LogTemp, Warning, TEXT("Server Set Owner called."));
	SetNewOwner(ControllerIn);
}

void AWriteableChestActor::Server_SetRenderText_Implementation(const FText& TextIn)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_SetRenderText Called."));
	SetRenderText(TextIn);
	RenderText = TextIn;
}

bool AWriteableChestActor::Server_SetRenderText_Validate(const FText& TextIn)
{
	return PC != nullptr;
}

void AWriteableChestActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWriteableChestActor, RenderText);
}
