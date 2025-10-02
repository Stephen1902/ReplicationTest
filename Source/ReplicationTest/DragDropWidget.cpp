// Copyright 2025 DME Games

#include "DragDropWidget.h"
#include "InventoryComp.h"
#include "WorldObjects.h"
#include "Components/Image.h"

void UDragDropWidget::SetItemInfo(const UDataTable* DataTable, UInventoryComp* InventoryCompIn, FName ItemIDIn)
{
	InventoryCompToUse = InventoryCompIn;
	DataTableRowHandle.DataTable = DataTable;
	ItemID = ItemIDIn;
	
	if (ItemID != "" && !DataTableRowHandle.IsNull())
	{
		if (const FWorldItemStruct* Row = DataTableRowHandle.DataTable->FindRow<FWorldItemStruct>(ItemID, ""))
		{
			ItemIcon->SetBrushFromTexture(Row->ItemIcon);
		}
	}
}
