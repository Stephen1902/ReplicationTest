// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", meta=(BindWidget))
	class UInventoryGrid* InventoryGrid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", meta=(BindWidget))
	class UTextBlock* InfoTextBlock;
public:
	void SetInventoryInfo(class UInventoryComp* InventoryCompIn);

	void SetInventoryVisibility(bool InventoryIsVisible);

	void ShowContainer(TSubclassOf<UUserWidget> WidgetToShow, UInventoryComp* ContainerInventory, class AReplicationTestCharacter* Player);
	
	void SetTextBlock(const FText& TextIn) const;

private:
	UPROPERTY()
	class UChestActorWidget* ChestWidgetRef;
};
