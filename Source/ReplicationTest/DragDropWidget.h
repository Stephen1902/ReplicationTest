// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "DragDropWidget.generated.h"

class UInventoryComp;

/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UDragDropWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drag Drop Widget", meta=(BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drag Drop Widget")
	UInventoryComp* InventoryCompToUse;

public:
	//void SetInventoryInfo(UDataTable* DataTable, UInventoryComp* InventoryCompIn);
	void SetItemInfo(const UDataTable* DataTable, UInventoryComp* InventoryCompIn, FName ItemIDIn);

private:
	UPROPERTY()
	FDataTableRowHandle DataTableRowHandle;

	FName ItemID;
};
