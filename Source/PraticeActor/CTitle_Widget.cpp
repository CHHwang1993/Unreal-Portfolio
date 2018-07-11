// Fill out your copyright notice in the Description page of Project Settings.

#include "CTitle_Widget.h"
#include "Engine.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UCTitle_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton = WidgetTree->FindWidget<UButton>(FName("GameStart"));
	QuitButton = WidgetTree->FindWidget<UButton>(FName("GameQuit"));

	if(StartButton)
		StartButton->OnClicked.AddDynamic(this, &UCTitle_Widget::OnGameStart);
	if (QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &UCTitle_Widget::OnGameQuit);
}

void UCTitle_Widget::OnGameStart()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Lobby"));
}

void UCTitle_Widget::OnGameQuit()
{
	TEnumAsByte<EQuitPreference::Type> quit;
	quit = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), quit);
}
