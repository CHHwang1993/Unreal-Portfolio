// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CTitlePC.generated.h"

class UCTitle_Widget;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API ACTitlePC : public APlayerController
{
	GENERATED_BODY()

private:
	TSubclassOf<class UUserWidget> TSubTitleWidget;
	UCTitle_Widget* TitleWidget;
	class USoundCue* BattleCue;
	
private:
	ACTitlePC();

protected:
	virtual void BeginPlay() override;
	
	
};
