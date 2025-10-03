// Copyright 2025 DME Games

#include "InventoryGrid.h"
#include "InventoryComp.h"
#include "InventorySlot.h"
#include "Components/WrapBox.h"

void UInventoryGrid::Client_InventoryUpdated_Implementation()
{
	SetInventoryInfo(InventoryComp);
}

void UInventoryGrid::SetInventoryInfo(UInventoryComp* InventoryCompIn)
{
	// Empty the wrap box to start fresh
	InvGrid_WrapBox->ClearChildren();

	if (!InventoryComp)
	{
    	InventoryComp = InventoryCompIn;
		// Set up the binding for when the inventory is updated
		InventoryComp->OnInventoryUpdated.AddDynamic(this, &UInventoryGrid::Client_InventoryUpdated);

       	DataTableHandle.DataTable = InventoryComp->GetDataTable();
    }

	if (InventorySlotToUse && InventoryComp && DataTableHandle.DataTable)
	{
		TArray<FItemStruct> ItemArray = InventoryComp->GetInventory();
		for (int32 i = 0;  i < ItemArray.Num(); ++i)
		{
			UInventorySlot* NewWidget = CreateWidget<UInventorySlot>(GetWorld(), InventorySlotToUse);
			InvGrid_WrapBox->AddChild(NewWidget);

			NewWidget->SetInventoryInfo(DataTableHandle.DataTable, InventoryComp);
			NewWidget->SetItemInfo(ItemArray[i].ItemID, ItemArray[i].Quantity, i);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryInfo called in InventoryGrid, but InventorySlotToUse or InventoryComp is not valid."));
	}
}
