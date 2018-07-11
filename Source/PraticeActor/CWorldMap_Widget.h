// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWorldMap_Widget.generated.h"

class ACBattlePC;
class ACBattleGS;
class UImage;
class UTexture2D;
class UCanvasPanelSlot;
class UCCircle_Widget;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCWorldMap_Widget : public UUserWidget
{
	GENERATED_BODY()

private:
	ACBattlePC * BattlePC;
	ACBattleGS* BattleGS;
	UCanvasPanelSlot* Slot;
	UCanvasPanelSlot* CurrentSlot;
	UCanvasPanelSlot* TargetSlot;
	UTexture2D* AirplaneTexture;
	UTexture2D* MyIconTexture;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UImage* MyIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCCircle_Widget* CurrentCircle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCCircle_Widget* TargetCircle;

	FVector2D WorldPositionToWidgetPosition(FVector worldPosition,FVector mapSize);
	float WorldScaleToWidgetScale(float worldLength, FVector mapSize);
};
