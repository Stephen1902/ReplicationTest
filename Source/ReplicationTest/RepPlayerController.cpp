// Copyright 2025 DME Games

#include "RepPlayerController.h"
#include "PlayerWidget.h"
#include "ReplicationTestCharacter.h"
#include "WriteableChestActor.h"
#include "WriteableChestWidget.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"

ARepPlayerController::ARepPlayerController()
{
	ChestActorRef = nullptr;
	PlayerWidgetRef = nullptr;
	WriteableChestWidgetRef = nullptr;
	
	bInventoryOnScreen = false;

	static ConstructorHelpers::FClassFinder<UPlayerWidget> Widget(TEXT("/Game/FirstPersonCPP/Blueprints/UI/WBP_PlayerWidget"));
	if (Widget.Succeeded())
	{
		PlayerWidget = Widget.Class;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT(TEXT("/Game/WorldItems/DT_WorldItems"));
	if (DT.Succeeded())
	{
		GameDataTable = DT.Object;
	}

	OwningPlayer = nullptr;
	bCanLook = true;
}

void ARepPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//if (HasAuthority())
	//{
		Server_AddWidget();
	//}
/*	else
	{
		Client_AddWidget();
	}
*/
}

void ARepPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Setup input bindings
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ARepPlayerController::ToggleInventory);

	InputComponent->BindAxis("Turn", this, &ARepPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ARepPlayerController::LookUp);
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

void ARepPlayerController::Turn(float Val)
{
	if (bCanLook && Val != 0.f && IsLocalController())
	{
		AddYawInput(Val);
	}
}

void ARepPlayerController::LookUp(float Val)
{
	if (bCanLook && Val != 0.f && IsLocalController())
	{
		AddPitchInput(Val);
	}
}

void ARepPlayerController::Server_AddWidget_Implementation()
{
	Client_AddWidget();
}

void ARepPlayerController::Client_AddWidget_Implementation()
{
	if (PlayerWidget)
	{
		PlayerWidgetRef = CreateWidget<UPlayerWidget>(this, PlayerWidget);

		if (PlayerWidgetRef)
		{
			PlayerWidgetRef->AddToViewport();

			OwningPlayer = Cast<AReplicationTestCharacter>(GetCharacter());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Widget is valid but failed to get a create a PlayerWidgetRef from it."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AddWidget in client called by %s, but PlayerWidget is not valid"), *GetName());
	}
}

void ARepPlayerController::ToggleInventory()
{
	// Only run this code if a valid PlayerWidget is on screen
	if (PlayerWidgetRef)
	{
		// Invert whether the widget is on screen
		bInventoryOnScreen = !bInventoryOnScreen;
		// Toggle the visibility of the widget, mouse cursor and whether the player can move
		PlayerWidgetRef->SetInventoryVisibility(bInventoryOnScreen);
		SetShowMouseCursor(bInventoryOnScreen);
		OwningPlayer->SetCanMove(!bInventoryOnScreen);
		bCanLook = !bInventoryOnScreen;

		// Update the inventory component on screen
		if (bInventoryOnScreen && GameDataTable && OwningPlayer)
		{
			PlayerWidgetRef->SetInventoryInfo(GameDataTable, OwningPlayer->GetInventoryComp());
		}
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