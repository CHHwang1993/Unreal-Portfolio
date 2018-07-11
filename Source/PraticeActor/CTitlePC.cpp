// Fill out your copyright notice in the Description page of Project Settings.

#include "CTitlePC.h"
#include "CTitle_Widget.h"
#include "UserWidget.h"
#include "Sound/SoundCue.h"
#include "Engine.h"

ACTitlePC::ACTitlePC()
{
	static ConstructorHelpers::FObjectFinder<UClass> titleWidget(TEXT("/Game/GameMode/Title/WidgetBP_Title.WidgetBP_Title_C"));
	if (titleWidget.Object)
		TSubTitleWidget = titleWidget.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> battleCue(TEXT("/Game/BGM/BattleGround_Cue.BattleGround_Cue"));
	if (battleCue.Succeeded())
		BattleCue = battleCue.Object;
}

void ACTitlePC::BeginPlay()
{
	UGameplayStatics::PlaySound2D(GetWorld(), BattleCue);

	if (TSubTitleWidget)
	{
		TitleWidget = CreateWidget<UCTitle_Widget>(GetWorld(), TSubTitleWidget);
		TitleWidget->AddToViewport();

		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(TitleWidget->GetCachedWidget());
		bShowMouseCursor = true;
		this->SetInputMode(Mode);
	}
}
