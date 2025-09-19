// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WriteableChestWidget.generated.h"

class UVerticalBox;
class UEditableText;
class UButton;
class ARepPlayerController;


/**
 * 
 */
UCLASS()
class REPLICATIONTEST_API UWriteableChestWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Writeable Box Widget", meta=(BindWidget))
	UVerticalBox* WBW_VerticalBox;

	UPROPERTY(EditDefaultsOnly, Category = "Writeable Box Widget", meta=(BindWidget))
	UEditableText* WBW_EditableText;

	UPROPERTY(EditDefaultsOnly, Category = "Writeable Box Widget", meta=(BindWidget))
	UButton* WBW_Button;

	UPROPERTY(VisibleAnywhere, Category = "Writable Box Widget")
	ARepPlayerController* PlayerControllerRef;

private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnButtonClicked();
public:
	void SetControllerAndChestRef(ARepPlayerController* ControllerIn, class AWriteableChestActor* ChestActorIn);
};
