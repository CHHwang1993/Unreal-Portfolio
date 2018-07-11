// Fill out your copyright notice in the Description page of Project Settings.

#include "CLoadingPC.h"
#include "CLoading_Widget.h"
#include "UserWidget.h"
#include "Engine.h"

ACLoadingPC::ACLoadingPC()
{
	static ConstructorHelpers::FObjectFinder<UClass> loadingWidget(TEXT("/Game/GameMode/Loading/WidgetBP_Loading.WidgetBP_Loading_C"));
	if (loadingWidget.Succeeded())
		TSubLoadingWidget = loadingWidget.Object;
}

void ACLoadingPC::BeginPlay()
{
	Super::BeginPlay();

	LoadingWidget = CreateWidget<UCLoading_Widget>(GetWorld(), TSubLoadingWidget);

	if (LoadingWidget)
	{
		LoadingWidget->AddToViewport();

		FString LevelName = UGameplayStatics::ParseOption(UGameplayStatics::GetGameMode(GetWorld())->OptionsString, FString("NextMap"));

		UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
	}
		
}
