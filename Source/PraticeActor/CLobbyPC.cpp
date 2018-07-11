// Fill out your copyright notice in the Description page of Project Settings.

#include "CLobbyPC.h"
#include "CLobby_Widget.h"
#include "Sound/SoundCue.h"
#include "UserWidget.h"
#include "Engine.h"

ACLobbyPC::ACLobbyPC()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> battleCue(TEXT("/Game/BGM/BattleGround_Cue.BattleGround_Cue"));
	if (battleCue.Succeeded())
		BattleCue = battleCue.Object;

	static ConstructorHelpers::FObjectFinder<UClass> lobbyWidget(TEXT("/Game/GameMode/Lobby/WidgetBP_Lobby.WidgetBP_Lobby_C"));
	if (lobbyWidget.Object)
		TSubLobbyWidget = lobbyWidget.Object;
}

void ACLobbyPC::BeginPlay()
{
	UGameplayStatics::PlaySound2D(GetWorld(), BattleCue);

	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MainCamera"), outActors);

	if (outActors.Num()>0)
	{
		ACameraActor* camera = Cast<ACameraActor>(outActors[0]);

		if (camera)
			this->SetViewTargetWithBlend(camera);
	}

	LobbyWidget = CreateWidget<UCLobby_Widget>(GetWorld(), TSubLobbyWidget);
	if (LobbyWidget)
	{
		LobbyWidget->AddToViewport();
		FInputModeUIOnly mode;
		mode.SetWidgetToFocus(LobbyWidget->GetCachedWidget());
		SetInputMode(mode);
		bShowMouseCursor = true;
	}

}

void ACLobbyPC::JoinServer(FString ipAdress)
{
	FString NextMap;
	FString option = TEXT("NextMap=");
	NextMap = option + ipAdress;
	UGameplayStatics::OpenLevel(GetWorld(), FName("Loading"),true, NextMap);
}
