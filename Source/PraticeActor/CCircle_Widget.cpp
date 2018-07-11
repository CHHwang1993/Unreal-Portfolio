// Fill out your copyright notice in the Description page of Project Settings.

#include "CCircle_Widget.h"
#include "WidgetBlueprintLibrary.h"
#include "WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "CanvasPanelSlot.h"

void UCCircle_Widget::OnPaint(UPARAM(ref) FPaintContext& Context) 
{
	Super::OnPaint(Context);

	/*UCanvasPanelSlot* slot =  UWidgetLayoutLibrary::SlotAsCanvasSlot(this);

	Center = slot->GetSize()*0.5f;

	fRadius = slot->GetSize().X *0.5f;

	for (int i = 0; i < 36; i++)
	{
		FVector vector = FVector(0.0f, fRadius, 0.0f);
		FVector vector1 = UKismetMathLibrary::RotateAngleAxis(vector, i*10.0f, FVector(0.0f, 0.0f, 1.0f));
		FVector vector2 = UKismetMathLibrary::RotateAngleAxis(vector, (i+1)*10.0f, FVector(0.0f, 0.0f, 1.0f));

		FVector2D positionA = FVector2D(vector1.X, vector1.Y);
		FVector2D positionB = FVector2D(vector2.X, vector2.Y);
		UWidgetBlueprintLibrary::DrawLine(Context, positionA, positionB);
	}*/

	
}
