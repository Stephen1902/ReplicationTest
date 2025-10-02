// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DD_Operation.generated.h"

class UInventoryComp;

/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UDD_Operation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	void SetItemInfo(UInventoryComp* InventoryCompIn, const int32 ItemIndexIn);

private:
	UPROPERTY()
	UInventoryComp* InventoryComp;
	int32 ItemIndex;
};
