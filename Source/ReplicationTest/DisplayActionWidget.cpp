// Copyright 2025 DME Games

#include "DisplayActionWidget.h"
#include "Components/TextBlock.h"

void UDisplayActionWidget::SetItemText(FText ItemTextIn)
{
	if (!ItemTextIn.IsEmpty())
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemInfoText->SetText(ItemTextIn);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}
