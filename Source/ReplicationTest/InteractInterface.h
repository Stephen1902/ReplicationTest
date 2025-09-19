// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class AReplicationTestCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REPLICATIONTEST_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact|ToInteract")
	FText OnActorViewed();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact|ToInteract")
	void OnActorViewLost();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact|ToInteract")
	void OnActorInteracted(AReplicationTestCharacter* PlayerWhoInteracted);	
};
