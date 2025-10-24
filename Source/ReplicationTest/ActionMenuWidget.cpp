// Copyright 2025 DME Games

#include "ActionMenuWidget.h"

#include "InventoryComp.h"
#include "InventorySlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"

void UActionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UCanvasPanelSlot* CPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(ActionMenuBox);
	CPS->SetPosition(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()));

	ButtonUse->OnClicked.AddDynamic(this, &UActionMenuWidget::OnUseClicked);
	ButtonDropOne->OnClicked.AddDynamic(this, &UActionMenuWidget::OnDropOneClicked);
	ButtonDropAll->OnClicked.AddDynamic(this, &UActionMenuWidget::OnDropAllClicked);
}

void UActionMenuWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (InventorySlot)
	{
		InventorySlot->RemoveActionMenu();
	}
}

void UActionMenuWidget::OnUseClicked()
{
	if (InventoryComp)
	{
		InventoryComp->Server_Remove(ItemIndex, false, true);
	}
}

void UActionMenuWidget::OnDropOneClicked()
{
	if (InventoryComp)
	{
		InventoryComp->Server_Remove(ItemIndex, false, false);
	}
}

void UActionMenuWidget::OnDropAllClicked()
{
	if (InventoryComp)
	{
		InventoryComp->Server_Remove(ItemIndex, true, false);
	}
}

void UActionMenuWidget::SetItemInfo(UInventoryComp* InventoryCompIn, int32 ItemIndexIn, UInventorySlot* InventorySlotIn)
{
	InventoryComp = InventoryCompIn;
	ItemIndex = ItemIndexIn;
	
	InventorySlot = InventorySlotIn;
}
