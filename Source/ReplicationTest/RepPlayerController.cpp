// Copyright 2025 DME Games

#include "RepPlayerController.h"

#include "PlayerWidget.h"
#include "WriteableChestActor.h"
#include "WriteableChestWidget.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

ARepPlayerController::ARepPlayerController()
{
	bInventoryOnScreen = false;
}

void ARepPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerWidget)
	{
		PlayerWidgetRef = CreateWidget<UPlayerWidget>(this, PlayerWidget);
		PlayerWidgetRef->AddToViewport();
	}
}

void ARepPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Setup input bindings
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ARepPlayerController::ToggleInventory);
}

void ARepPlayerController::DealWithWriteableChest(AWriteableChestActor* ChestIn, TSubclassOf<UWriteableChestWidget> WidgetIn)
{
	if (ChestIn && WidgetIn)
	{
		if (!HasAuthority())
		{
			Server_DealWithWriteableChest(ChestIn);
		}
		else
		{
			ChestActorRef = ChestIn;
		}

		WriteableChestWidgetRef = CreateWidget<UWriteableChestWidget>(this, WidgetIn);
		WriteableChestWidgetRef->AddToViewport();
		WriteableChestWidgetRef->SetControllerAndChestRef(this, ChestIn);
		WriteableChestWidgetRef->SetFocus();
		                            		
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Deal With Writable Chest called on %s but either ChestIn or WidgetIn is not valid."), *GetName());
	}
}

// Has to be a server function so it can be the server when setting the text on the relevant actor
void ARepPlayerController::DealWithChestWidgetClosed(const FText& TextIn)
{
	if (!HasAuthority())
	{
		Server_DealWithChestWidgetClosed(TextIn);
	}
	else
	{
		if (ChestActorRef)
		{
			ChestActorRef->SetRenderText(TextIn);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DealWithChestWidgetClosed: No Chest Actor Ref."));
		}
	}

	if (WriteableChestWidgetRef)
	{
		WriteableChestWidgetRef->RemoveFromParent();
	}
	
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}

void ARepPlayerController::ToggleInventory()
{
	// Only run this code if a valid PlayerWidget is on screen
	if (PlayerWidgetRef)
	{
		bInventoryOnScreen = !bInventoryOnScreen;

		PlayerWidgetRef->SetInventoryVisibility(bInventoryOnScreen);
	}
}

void ARepPlayerController::Server_DealWithWriteableChest_Implementation(AWriteableChestActor* ChestIn)
{
	ChestActorRef = ChestIn;
}

void ARepPlayerController::Server_DealWithChestWidgetClosed_Implementation(const FText& TextIn)
{
	DealWithChestWidgetClosed(TextIn);
}

void ARepPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARepPlayerController, ChestActorRef);
}