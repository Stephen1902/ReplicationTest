// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WorldObjects.generated.h"

USTRUCT()
struct FWorldItemStruct : public FTableRowBase
{
	GENERATED_BODY();

	// Item Name as displayed on screen
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Items")
	FName ItemName;

	// Item Description as displayed on screen
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Items")
	FName ItemDescription;

	// Icon to display in an inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Items")
	UTexture2D* ItemIcon;

	// Class of item to be created in the world when "dropped"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Items")
	TSubclassOf<AActor> ActorToSpawn;

	// The maximum that can be held in one inventory slot
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Items")
	int32 ItemMaxStack;
	
	FWorldItemStruct()
	{
		ItemName = FName("Test");
		ItemDescription = FName("Item Description");
		ActorToSpawn = nullptr;
		ItemIcon = nullptr;
		ItemMaxStack = 1;
	}
};

/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UWorldObjects : public UObject
{
	GENERATED_BODY()

};
