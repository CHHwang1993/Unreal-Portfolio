// Fill out your copyright notice in the Description page of Project Settings.

#include "CWorldMap_Widget.h"
#include "CCircle_Widget.h"
#include "CBattlePC.h"
#include "CBattleGS.h"
#include "Components/Image.h"
#include "CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "WidgetLayoutLibrary.h"
#include "Engine.h"
#include "Engine/Texture2D.h"
#include "Kismet/KismetMathLibrary.h"

void UCWorldMap_Widget::NativeConstruct()
{
	BattlePC = Cast<ACBattlePC>(GetOwningPlayer());
	BattleGS = Cast<ACBattleGS>(UGameplayStatics::GetGameState(GetWorld()));

	MyIcon = WidgetTree->FindWidget<UImage>(FName("MyIcon"));

	CurrentCircle = WidgetTree->FindWidget<UCCircle_Widget>(FName("CurrentCircle"));
	TargetCircle = WidgetTree->FindWidget<UCCircle_Widget>(FName("TargetCircle"));

	CurrentSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CurrentCircle);
	TargetSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TargetCircle);
	Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(MyIcon);

	/*static ConstructorHelpers::FObjectFinder<UTexture2D> airplaneTexture(TEXT("Texture2D'/Game/WorldMap/T_C130Icon.T_C130Icon'"));
	if (airplaneTexture.Succeeded())
		AirplaneTexture = airplaneTexture.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> myIconTexture(TEXT("Texture2D'/Game/WorldMap/T_MyIcon.T_MyIcon'"));
	if (myIconTexture.Succeeded())
		MyIconTexture = myIconTexture.Object;*/
}

void UCWorldMap_Widget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	if (BattlePC)
	{
		FVector2D widgetSize = WorldPositionToWidgetPosition(BattlePC->GetPawnPosition(), BattlePC->GetMapSize());

		/*if (BattlePC->GetAirplane())
		{
			if (AirplaneTexture)
			{
				MyIcon->SetBrushFromTexture(AirplaneTexture, true);
			}
			
		}
		else
		{
			if (MyIconTexture)
			{
				MyIcon->SetBrushFromTexture(MyIconTexture, true);
			}
		}*/

		if (Slot)
		{
			Slot->SetPosition(widgetSize);
		}
	
		float angle;
		FVector crossDirection = FVector::CrossProduct(FVector(0.0f, -1.0f, 0.0f), BattlePC->GetPawnDirection());

		if (crossDirection.Z >= 0)
			angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(FVector(0.0f, -1.0f, 0.0f), BattlePC->GetPawnDirection()));
		else
			angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(FVector(0.0f, -1.0f, 0.0f), BattlePC->GetPawnDirection()))*-1.0f;

		
		MyIcon->SetRenderAngle(angle);

		if (BattleGS->bVisibleTargetCircle)
			TargetCircle->SetVisibility(ESlateVisibility::Visible);
		else
			TargetCircle->SetVisibility(ESlateVisibility::Collapsed);

		if (BattleGS->bVisibleCurrentCircle)
			CurrentCircle->SetVisibility(ESlateVisibility::Visible);
		else
			CurrentCircle->SetVisibility(ESlateVisibility::Collapsed);

		
		if (CurrentSlot)
		{
			FVector2D currentSize;
			float scale = WorldScaleToWidgetScale(BattleGS->CurrentCircleRadius, BattlePC->GetMapSize()) * 2;
			FVector2D position = WorldPositionToWidgetPosition(BattleGS->CurrentCirclePosition, BattlePC->GetMapSize());
			currentSize = FVector2D(scale, scale);
			CurrentSlot->SetPosition(position);
			CurrentSlot->SetSize(currentSize);
		}

		
		if (TargetSlot)
		{
			FVector2D targetSize;
			float scale = WorldScaleToWidgetScale(BattleGS->TargetCircleRadius, BattlePC->GetMapSize()) * 2;
			FVector2D position = WorldPositionToWidgetPosition(BattleGS->TargetCirclePosition, BattlePC->GetMapSize());
			targetSize = FVector2D(scale, scale);
			TargetSlot->SetPosition(position);
			TargetSlot->SetSize(targetSize);
		}
		
	}
}

float UCWorldMap_Widget::WorldScaleToWidgetScale(float worldLength, FVector mapSize)
{
	float widgetLength = 1000.0f/ mapSize.X *worldLength;

	return widgetLength;
}

FVector2D UCWorldMap_Widget::WorldPositionToWidgetPosition(FVector worldPosition, FVector mapSize)
{
	FVector mapVector = worldPosition + (mapSize * 0.5f);

	FVector2D widgetSize = FVector2D((mapVector.X / mapSize.X)*1000.0f, (mapVector.Y / mapSize.Y)*1000.0f);

	return widgetSize;
}

