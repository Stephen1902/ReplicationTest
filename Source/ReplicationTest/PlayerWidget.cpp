// Copyright 2025 DME Games

#include "PlayerWidget.h"
#include "ChestActorWidget.h"
#include "InventoryGrid.h"
#include "ReplicationTestCharacter.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPlayerWidget::SetInventoryInfo(UInventoryComp* InventoryCompIn)
{
	if (InventoryCompIn)
	{
		InventoryGrid->SetInventoryInfo(InventoryCompIn);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryInfo called on PlayerWidget.  DT or IC."))
	}
}

void UPlayerWidget::SetInventoryVisibility(bool InventoryIsVisible)
{
	if (InventoryIsVisible)
	{
		// See if there is a widget on screen already, remove it if there is
		if (ChestWidgetRef)
		{
			ChestWidgetRef->RemoveFromParent();
			ChestWidgetRef = nullptr;
		}
		
		InventoryGrid->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InventoryGrid->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerWidget::ShowContainer(TSubclassOf<UUserWidget> WidgetToShow, UInventoryComp* ContainerInventory, AReplicationTestCharacter* Player)
{
	ChestWidgetRef = CreateWidget<UChestActorWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), WidgetToShow);
	ChestWidgetRef->AddToViewport();
	ChestWidgetRef->SetItemInfo(Player->GetInventoryComp(), ContainerInventory);
}


void UPlayerWidget::SetTextBlock(const FText& TextIn) const
{
	InfoTextBlock->SetText(TextIn);
}
