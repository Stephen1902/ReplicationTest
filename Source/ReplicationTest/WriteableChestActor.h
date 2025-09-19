// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "WriteableChestActor.generated.h"

class UWriteableChestWidget;

UCLASS()
class REPLICATIONTEST_API AWriteableChestActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWriteableChestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Writeable Chest")
	USceneComponent* RootComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Writeable Chest")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Writeable Chest")
	class UTextRenderComponent* TextRenderComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Writeable Chest")
	TSubclassOf<UWriteableChestWidget> WidgetToDisplay;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintGetter, Category = "Writeable Chest")
	FText GetRenderText() const { return RenderText; }

	//UFUNCTION()
	void SetRenderText(const FText& TextIn);
	
	void WidgetHasBeenClosed();

	void SetNewOwner(class ARepPlayerController* ControllerIn);
private:
	UPROPERTY(ReplicatedUsing=OnRep_RenderTextUpdated)
	FText RenderText;

	UFUNCTION()
	void OnRep_RenderTextUpdated();

	virtual FText OnActorViewed_Implementation() override;
	virtual void OnActorViewLost_Implementation() override;
	virtual void OnActorInteracted_Implementation(AReplicationTestCharacter* CharacterInteracting) override;

	UPROPERTY()
	class ARepPlayerController* PC;
	
	UPROPERTY()
	UWriteableChestWidget* WriteableChestWidgetRef;

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SetRenderText(const FText& TextIn);

	UFUNCTION(Server, Reliable)
	void Server_SetOwner(ARepPlayerController* ControllerIn);
};
