// Copyright 2025 DME Games

#include "InventoryGrid.h"
#include "InventoryComp.h"
#include "InventorySlot.h"
#include "Components/WrapBox.h"

void UInventoryGrid::SetInventoryInfo(UInventoryComp* InventoryCompIn, UDataTable* DataTableIn)
{
	// Empty the wrap box to start fresh
	InvGrid_WrapBox->ClearChildren();

	if (!InventoryComp)
	{
    	InventoryComp = InventoryCompIn;
    }
	
	if (InventorySlotToUse && InventoryComp)
	{
		TArray<FItemStruct> ItemArray = InventoryComp->GetInventory();
		for (FItemStruct It : ItemArray)
		{
			UInventorySlot* NewWidget = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotToUse);
			InvGrid_WrapBox->AddChild(NewWidget);

			NewWidget->SetInventoryInfo(DataTableIn, InventoryComp);
			NewWidget->SetItemInfo(It.ItemID, It.Quantity);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryInfo called in InventoryGrid, but InventorySlotToUse or InventoryComp is not valid."));
	}
}
