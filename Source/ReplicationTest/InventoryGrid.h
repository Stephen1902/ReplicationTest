// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "InventoryGrid.generated.h"

class UBorder;
class UScrollBox;
class UWrapBox;
class UInventorySlot;
class UInventoryComp;
class UDataTable;

/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Grid", meta=(BindWidget))
	UBorder* InvGrid_Border;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UScrollBox* InvGrid_ScrollBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot", meta=(BindWidget))
	UWrapBox* InvGrid_WrapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Slot")
	TSubclassOf<UInventorySlot> InventorySlotToUse;
private:
	UPROPERTY()
	UInventoryComp* InventoryComp;

	UPROPERTY()
	FDataTableRowHandle DataTableHandle;

	UFUNCTION(Server, Reliable)
	void Server_SetInventoryInfo(UInventoryComp* InventoryCompToUse);
	
	UFUNCTION(Client, Unreliable)
	void Client_InventoryUpdated();

public:
	void SetInventoryInfo(UInventoryComp* InventoryCompIn);
};
