// Copyright 2025 DME Games

#include "PlayerWidget.h"
#include "InventoryGrid.h"

void UPlayerWidget::SetInventoryInto(class UDataTable* DataTableIn, class UInventoryComp* InventoryCompIn)
{
	if (DataTableIn && InventoryCompIn)
	{
		InventoryGrid->SetInventoryInfo(InventoryCompIn, DataTableIn);
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
