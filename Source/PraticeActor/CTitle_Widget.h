// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTitle_Widget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCTitle_Widget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* StartButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* QuitButton;

	UFUNCTION()
		void OnGameStart();
	UFUNCTION()
		void OnGameQuit();
};	
