// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CClientMain_Widget.generated.h"

class ACBattlePC;
class ACBattleGS;
class UTextBlock;
class UProgressBar;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCClientMain_Widget : public UUserWidget
{
	GENERATED_BODY()

private:
	ACBattlePC* PlayerController;
	ACBattleGS* GameState;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* ClientText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* AliveCountText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProgressBar* HPBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UWidgetAnimation* BloodEffectAnimation;
	
};
