// Copyright 2025 DME Games

#include "WriteableChestWidget.h"
#include "RepPlayerController.h"
#include "WriteableChestActor.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

void UWriteableChestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WBW_EditableText->OnTextCommitted.AddDynamic(this, &UWriteableChestWidget::OnTextCommited);
	WBW_Button->OnClicked.AddDynamic(this, &UWriteableChestWidget::OnButtonClicked);
}

void UWriteableChestWidget::OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (!Text.EqualTo(WBW_EditableText->GetText()))
	{
	
	}

}

void UWriteableChestWidget::OnButtonClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("Text is %s"), *WBW_EditableText->GetText().ToString());
	if (PlayerControllerRef)
	{
		PlayerControllerRef->DealWithChestWidgetClosed(WBW_EditableText->GetText());
		//ChestActorRef->WidgetHasBeenClosed();
	}
}

void UWriteableChestWidget::SetControllerAndChestRef(ARepPlayerController* ControllerIn, AWriteableChestActor* ChestActorIn)
{
	PlayerControllerRef = ControllerIn;
	WBW_EditableText->SetText(ChestActorIn->GetRenderText());
}
/*
void UWriteableChestWidget::SetChestActorRef(AWriteableChestActor* ActorIn)
{
	ChestActorRef = ActorIn;

	if (ChestActorRef)
	{
		WBW_EditableText->SetText(ChestActorRef->GetRenderText());
		WBW_EditableText->SetFocus();
	}
}
*/