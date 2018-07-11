// Fill out your copyright notice in the Description page of Project Settings.

#include "CResult_Widget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine.h"

void UCResult_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	ResultText = WidgetTree->FindWidget<UTextBlock>(FName("ResultText"));
	RankText = WidgetTree->FindWidget<UTextBlock>(FName("RankText"));
	LobbyButton = WidgetTree->FindWidget<UButton>(FName("GoToLobby"));
	LobbyButton->OnClicked.AddDynamic(this, &UCResult_Widget::OnGoToLobby);
}

void UCResult_Widget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ResultText->SetText(FText::FromString(ResultString));
	RankText->SetText(FText::FromString(RankString));
}

void UCResult_Widget::OnGoToLobby()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Lobby"));
}
