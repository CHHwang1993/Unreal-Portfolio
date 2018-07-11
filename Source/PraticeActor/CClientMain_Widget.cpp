// Fill out your copyright notice in the Description page of Project Settings.

#include "CClientMain_Widget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Animation/WidgetAnimation.h"
#include "CBattlePC.h"
#include "CBattleGS.h"
#include "CPlayer.h"
#include "Engine.h"

void UCClientMain_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	ClientText = WidgetTree->FindWidget<UTextBlock>(FName("ClientText"));
	AliveCountText = WidgetTree->FindWidget<UTextBlock>(FName("AliveCount"));
	HPBar = WidgetTree->FindWidget<UProgressBar>(FName("HPBar"));

	PlayerController = Cast<ACBattlePC>(GetOwningPlayer());

	GameState = Cast<ACBattleGS>(UGameplayStatics::GetGameState(GetWorld()));

	int a = 0;
}

void UCClientMain_Widget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(PlayerController && ClientText)
		ClientText->SetText(FText::FromString(PlayerController->NotificationFromServer));
	if (AliveCountText && GameState)
	{
		AliveCountText->SetText(FText::FromString(FString::FromInt(GameState->nAliveCount)));
	}

	if (PlayerController)
	{
		ACPlayer* player = Cast<ACPlayer>(PlayerController->GetPawn());

		if (HPBar)
		{
			if (player)
			{
				float percent = player->GetHP() / player->GetMaxHP();
				HPBar->SetPercent(percent);
			}
			else
				HPBar->SetPercent(0.0f);
		}
		
	}
}
