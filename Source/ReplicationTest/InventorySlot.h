// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "InventorySlot.generated.h"

class USizeBox;
class UButton;
class UBorder;
class UOverlay;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	USizeBox* InvSlot_SizeBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UButton* InvSlot_Button;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UBorder* InvSlot_OuterBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UBorder* InvSlot_InnerBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UOverlay* InvSlot_Overlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UImage* InvSlot_Image;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	USizeBox* InvSlot_QtySizeBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UBorder* InvSlot_QtyOuterBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UBorder* InvSlot_QtyInnerBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UTextBlock* InvSlot_QtyTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot")
	TSubclassOf<class UDragDropWidget> DragDropWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	FName ItemID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	int32 ItemIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	class UInventoryComp* InventoryComp;

public:
	void SetInventoryInfo(UDataTable* DataTable, UInventoryComp* InventoryCompIn);
	void SetItemInfo(FName ItemIDIn, int32 QuantityIn, int32 IndexIn);

private:
	UPROPERTY()
	FDataTableRowHandle DataTableRowHandle;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation ) override;
};
