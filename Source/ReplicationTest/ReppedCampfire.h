// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReppedCampfire.generated.h"

UCLASS()
class REPLICATIONTEST_API AReppedCampfire : public AActor
{
	GENERATED_BODY()
	
protected:	
	// Sets default values for this actor's properties
	AReppedCampfire();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Campfire")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Campfire")
	class USphereComponent* CollisionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Campfire")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Campfire")
	UParticleSystemComponent* ParticleSystemComp;
/*	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Campfire")
	UParticleSystem* ParticleToSpawn;
	
	UFUNCTION(NetMulticast, Reliable)
	void ChangeLitMode();
*/	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
public:	
	// Called every frame
//	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(ReplicatedUsing=OnRep_ChangeLitMode)
	bool bIsLit;

	UFUNCTION()
	void OnRep_ChangeLitMode() const;

};
