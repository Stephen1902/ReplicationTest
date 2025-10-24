// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionMenuWidget.generated.h"

class UCanvasPanel;
class UVerticalBox;
class UButton;
class UInventoryComp;

/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UActionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widgets", meta=(BindWidget))
	UCanvasPanel* ActionMenuPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widgets", meta=(BindWidget))
	UVerticalBox* ActionMenuBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widgets", meta=(BindWidget))
	UButton* ButtonUse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widgets", meta=(BindWidget))
	UButton* ButtonDropOne;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widgets", meta=(BindWidget))
	UButton* ButtonDropAll;

public:
	void SetItemInfo(UInventoryComp* InventoryCompIn, int32 ItemIndexIn, class UInventorySlot* InventorySlotIn);

private:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY()
	UInventoryComp* InventoryComp;

	UPROPERTY()
	UInventorySlot* InventorySlot;
	
	int32 ItemIndex;

	UFUNCTION()
	void OnUseClicked();
	UFUNCTION()
	void OnDropOneClicked();
	UFUNCTION()
	void OnDropAllClicked();
};
