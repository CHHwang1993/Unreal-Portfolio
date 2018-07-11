// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInteraction_Widget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCInteraction_Widget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* Interaction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString FoundFieldItemName;

};
