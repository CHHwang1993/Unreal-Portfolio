// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CCircle_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCCircle_Widget : public UUserWidget
{
	GENERATED_BODY()
		
private:
	FVector2D Center;
	float fRadius;
public:
	virtual void OnPaint(UPARAM(ref) FPaintContext& Context);
	
};
