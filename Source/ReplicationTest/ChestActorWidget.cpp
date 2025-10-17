// Copyright 2025 DME Games


#include "ChestActorWidget.h"
#include "InventoryComp.h"
#include "InventoryGrid.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "RepPlayerController.h"
#include "Components/Button.h"

void UChestActorWidget::NativePreConstruct()
{
	Super::NativePreConstruct();


}

void UChestActorWidget::SetItemInfo(UInventoryComp* PlayerInventoryIn, UInventoryComp* ChestInventoryIn)
{
	const ACharacter* PC = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PC)
    {
    	UInventoryComp* InventoryComp = Cast<UInventoryComp>(PC->GetComponentByClass(UInventoryComp::StaticClass()));
    	if (InventoryComp)
    	{
    		PlayerInventory->SetInventoryInfo(PlayerInventoryIn);
    	}
    }

	if (ChestInventoryIn)
	{
		ChestInventory->SetInventoryInfo(ChestInventoryIn);
	}
}

void UChestActorWidget::SetPlayerController(ARepPlayerController* ControllerIn)
{
	if (ControllerIn)
	{
		ExitButton->OnClicked.AddDynamic(ControllerIn, &ARepPlayerController::RemoveContainer);
	}
}
