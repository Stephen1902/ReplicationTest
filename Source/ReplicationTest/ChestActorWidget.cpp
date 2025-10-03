// Copyright 2025 DME Games


#include "ChestActorWidget.h"
#include "InventoryComp.h"
#include "InventoryGrid.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UChestActorWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	const ACharacter* PC = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PC)
	{
		UInventoryComp* InventoryComp = Cast<UInventoryComp>(PC->GetComponentByClass(UInventoryComp::StaticClass()));
		if (InventoryComp)
		{
			PlayerInventory->SetInventoryInfo(InventoryComp);
		}
	}
}
