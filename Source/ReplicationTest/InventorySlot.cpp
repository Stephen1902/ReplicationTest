// Copyright 2025 DME Games

#include "InventorySlot.h"

#include "DD_Operation.h"
#include "DragDropWidget.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "InventoryComp.h"
#include "WorldObjects.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetInputLibrary.h"

void UInventorySlot::SetInventoryInfo(const UDataTable* DataTable, UInventoryComp* InventoryCompIn)
{
	InventoryComp = InventoryCompIn;
	DataTableRowHandle.DataTable = DataTable;

	if (!InventoryComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryInfo was called in InventorySlot but without a valid Inventory Comp"));
	}

	if (!DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetInventoryInfo was called in InventorySlot but without a valid Data Table"));
	}
}


void UInventorySlot::SetItemInfo(FName ItemIDIn, int32 QuantityIn, int32 IndexIn)
{
	ItemID = ItemIDIn;
	ItemIndex = IndexIn;
	
	InvSlot_Image->SetVisibility(ESlateVisibility::Hidden);
    InvSlot_QtySizeBox->SetVisibility(ESlateVisibility::Hidden);
	
	if (ItemID != "" && QuantityIn > 0 && !DataTableRowHandle.IsNull())
	{
		
		if (const FWorldItemStruct* Row = DataTableRowHandle.DataTable->FindRow<FWorldItemStruct>(ItemID, ""))
		{
			InvSlot_Image->SetBrushFromTexture(Row->ItemIcon);
			InvSlot_QtyTextBlock->SetText(FText::FromString(FString::FromInt(QuantityIn)));
			
			InvSlot_Image->SetVisibility(ESlateVisibility::Visible);
			InvSlot_QtySizeBox->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

FReply UInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// Make sure that the ItemId is valid (not blank) and that it is the left mouse button being pressed.
	if (ItemID != "" && UKismetInputLibrary::PointerEvent_IsMouseButtonDown(InMouseEvent, EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
		
	return FReply::Unhandled();
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragDropWidget)
	{
		UDragDropWidget* NewDragDropWidget = CreateWidget<UDragDropWidget>(GetWorld(), DragDropWidget);
		NewDragDropWidget->SetItemInfo(DataTableRowHandle.DataTable, InventoryComp, ItemID);
		UDD_Operation* NewDragDropOperation = Cast<UDD_Operation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDD_Operation::StaticClass()));
		NewDragDropOperation->SetItemInfo(InventoryComp, ItemIndex);
		NewDragDropOperation->DefaultDragVisual = NewDragDropWidget;

		OutOperation = NewDragDropOperation;
	}
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	const UDD_Operation* LocalOperation = Cast<UDD_Operation>(InOperation);
	if (LocalOperation)
	{
		// Check that the indexes are different or the inventory components are different
		if (LocalOperation->GetItemIndex() != ItemIndex || LocalOperation->GetInventoryComp() != InventoryComp)
		{
			InventoryComp->Server_TransferSlot_Implementation(LocalOperation->GetItemIndex(), LocalOperation->GetInventoryComp(), ItemIndex);
			return true;
		}
	}

	return false;
}

