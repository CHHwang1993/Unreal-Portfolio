// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CLoadingPC.generated.h"

class UCLoading_Widget;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API ACLoadingPC : public APlayerController
{
	GENERATED_BODY()
	
private:
	ACLoadingPC();

	TSubclassOf<class UUserWidget> TSubLoadingWidget;
	UCLoading_Widget* LoadingWidget;

protected:
	virtual void BeginPlay() override;
	
	
};
