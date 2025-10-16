// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChestActorWidget.generated.h"

class UInventoryGrid;
class UInventoryComp;
/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UChestActorWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chest Actor Widget", meta=(BindWidget))
	UInventoryGrid* ChestInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chest Actor Widget", meta=(BindWidget))
	UInventoryGrid* PlayerInventory;

private:
	virtual void NativePreConstruct() override;

public:
	void SetItemInfo(UInventoryComp* PlayerInventoryIn, UInventoryComp* ChestInventoryIn);
};
