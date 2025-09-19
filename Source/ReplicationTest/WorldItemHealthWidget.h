// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldItemHealthWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class REPLICATIONTEST_API UWorldItemHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealthWidget(float CurrentHealth, float MaxHealth) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "World Item Health Widget", meta=(BindWidget))
	UTextBlock* HealthTextBlock;

	UPROPERTY(EditDefaultsOnly, Category = "World Item Health Widget", meta=(BindWidget))
	UProgressBar* HealthProgressBar;
};
