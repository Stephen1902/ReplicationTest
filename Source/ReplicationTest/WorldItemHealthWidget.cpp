// Copyright 2025 DME Games

#include "WorldItemHealthWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWorldItemHealthWidget::UpdateHealthWidget(float CurrentHealth, float MaxHealth) const
{
	const FString StringToDisplay = FString::SanitizeFloat(CurrentHealth) + " / " + FString::SanitizeFloat(MaxHealth);
	HealthTextBlock->SetText(FText::FromString(StringToDisplay));

	const float HealthAsPercentage = 1.0f - ((MaxHealth - CurrentHealth) / CurrentHealth);
	HealthProgressBar->SetPercent(HealthAsPercentage);
}
