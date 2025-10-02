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
		for (int32 i = 0;  i < ItemArray.Num(); ++i)
		{
			UInventorySlot* NewWidget = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotToUse);
			InvGrid_WrapBox->AddChild(NewWidget);

			NewWidget->SetInventoryInfo(DataTableIn, InventoryComp);
			NewWidget->SetItemInfo(ItemArray[i].ItemID, ItemArray[i].Quantity, i);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryInfo called in InventoryGrid, but InventorySlotToUse or InventoryComp is not valid."));
	}
}
