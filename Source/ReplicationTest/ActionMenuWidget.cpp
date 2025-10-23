// Copyright 2025 DME Games

#include "ActionMenuWidget.h"
#include "InventorySlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"

void UActionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UCanvasPanelSlot* CPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(ActionMenuBox);
	CPS->SetPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()));
}

void UActionMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (InventorySlot)
	{
		InventorySlot->RemoveActionMenu();
	}
}

void UActionMenuWidget::SetItemInfo(UInventoryComp* InventoryCompIn, int32 ItemIndexIn, UInventorySlot* InventorySlotIn)
{
	InventoryComp = InventoryCompIn;
	ItemIndex = ItemIndexIn;
	
	InventorySlot = InventorySlotIn;
}