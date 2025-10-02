// Copyright 2025 DME Games

#include "PlayerWidget.h"
#include "InventoryGrid.h"
#include "Components/TextBlock.h"

void UPlayerWidget::SetInventoryInfo(UDataTable* DataTableIn, UInventoryComp* InventoryCompIn)
{
	if (DataTableIn && InventoryCompIn)
	{
		InventoryGrid->SetInventoryInfo(InventoryCompIn, DataTableIn);
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryInfo called on PlayerWidget"))
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
		InventoryGrid->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		InventoryGrid->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerWidget::SetTextBlock(const FText& TextIn) const
{
	InfoTextBlock->SetText(TextIn);
}
