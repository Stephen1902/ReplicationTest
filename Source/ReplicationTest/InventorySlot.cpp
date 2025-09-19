// Copyright 2025 DME Games

#include "InventorySlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "InventoryComp.h"
#include "WorldObjects.h"
#include "Components/TextBlock.h"

void UInventorySlot::SetInventoryInfo(UDataTable* DataTable, UInventoryComp* InventoryCompIn)
{
	InventoryComp = InventoryCompIn;
	DataTableRowHandle.DataTable = DataTable;

	if (!InventoryComp || DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryInfo was called in InventorySlot but without a valid Inventory Comp or Data Table"));
	}
}


void UInventorySlot::SetItemInfo(FName ItemIDIn, int32 QuantityIn)
{
	InvSlot_Image->SetVisibility(ESlateVisibility::Hidden);
    InvSlot_QtySizeBox->SetVisibility(ESlateVisibility::Hidden);
	
	if (ItemIDIn != "" && QuantityIn > 0 && !DataTableRowHandle.IsNull())
	{
		if (const FWorldItemStruct* Row = DataTableRowHandle.DataTable->FindRow<FWorldItemStruct>(ItemIDIn, ""))
		{
			InvSlot_Image->SetBrushFromTexture(Row->ItemIcon);
			InvSlot_QtyTextBlock->SetText(FText::FromString(FString::FromInt(QuantityIn)));
			
			InvSlot_Image->SetVisibility(ESlateVisibility::Visible);
			InvSlot_QtySizeBox->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
