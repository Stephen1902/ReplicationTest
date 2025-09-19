// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RepPlayerController.generated.h"

class AWriteableChestActor;
class UWriteableChestWidget;
class UPlayerWidget;

/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API ARepPlayerController : public APlayerController
{
	GENERATED_BODY()

	ARepPlayerController();
protected:
	// Called on Game Start
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Controller")
	TSubclassOf<UPlayerWidget> PlayerWidget;

public:
	void DealWithWriteableChest(AWriteableChestActor* ChestIn, TSubclassOf<UWriteableChestWidget> WidgetIn);
	void DealWithChestWidgetClosed(const FText& TextIn);
	
private:
	// Has to be a replicated variable to call the function as a valid "server" actor
	UPROPERTY(Replicated)
	AWriteableChestActor* ChestActorRef;

	UFUNCTION(Server, Reliable)
	void Server_DealWithWriteableChest(AWriteableChestActor* ChestIn);

	UPROPERTY()
	UPlayerWidget* PlayerWidgetRef;
	
	UPROPERTY()
	UWriteableChestWidget* WriteableChestWidgetRef;

	UFUNCTION(Server, Reliable)
	void Server_DealWithChestWidgetClosed(const FText& TextIn);

	void ToggleInventory();
	bool bInventoryOnScreen;
};
