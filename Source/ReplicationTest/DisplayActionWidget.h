// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DisplayActionWidget.generated.h"

/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UDisplayActionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display Action Widget", meta=(BindWidget))
	class UTextBlock* ItemInfoText;

public:
	void SetItemText(FText ItemTextIn);
};
