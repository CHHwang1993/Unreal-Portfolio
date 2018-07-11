// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLobby_Widget.generated.h"

class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCLobby_Widget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* JoinServerButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UEditableTextBox* IPAdressTextBox;

	UFUNCTION()
		void OnJoinServer();
	
};
