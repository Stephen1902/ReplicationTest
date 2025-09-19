// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantActor.generated.h"

UCLASS()
class REPLICATIONTEST_API APlantActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant Actors")
	USceneComponent* RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant Actors")
	class USphereComponent* CollisionComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant Actors")
	UStaticMeshComponent* MeshComp;
	
	// Time taken before next stage of growth takes place
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant Actors")
	float TimeBetweenNewStage;

	// Meshes to be displayed at each stage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant Actors")
	TArray<UStaticMesh*> MeshArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant Actors")
	float StartingHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Plant Actors")
	class UWidgetComponent* WidgetComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeHealth(const float HealthChange);

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(NetMulticast, Reliable)
	void NewStageTriggered();

	UPROPERTY(ReplicatedUsing=OnRepHealthChanged)
	float CurrentHealth;

	UFUNCTION()
	void OnRepHealthChanged();

	// Override Replicate Properties function
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	FTimerHandle NextStageTimer;
};
