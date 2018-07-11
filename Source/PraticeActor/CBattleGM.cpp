// Fill out your copyright notice in the Description page of Project Settings.

#include "CBattleGM.h"
#include "CBattlePC.h"
#include "CBattleGS.h"
#include "CAirplane_C130.h"
#include "CPlayer.h"
#include "Engine.h"


ACBattleGM::ACBattleGM()
	:PlayingState(EPlayingState::Waiting)
	, nMinimumPlayerCount(2), nRemainTimeToStart(15), nWaitingTime(15), nRemainTimeOfCeremony(10)
{
	bReplicates = true;

	PlayerControllerClass = ACBattlePC::StaticClass();
	DefaultPawnClass = ACPlayer::StaticClass();
	GameStateClass = ACBattleGS::StaticClass();
}

void ACBattleGM::BeginPlay()
{
	Super::BeginPlay();

	nRemainTimeToStart = nWaitingTime;

	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("C130"), outActors);

	if (outActors.Num() > 0)
		Airplane_C130 = Cast<ACAirplane_C130>(outActors[0]);

	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle,this, &ACBattleGM::MainTimer, 1.0f, true);
}

void ACBattleGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ACBattlePC* battlePC = Cast<ACBattlePC>(NewPlayer);

	if (battlePC)
	{
		if (PlayingState == EPlayingState::Waiting)
		{
			AlivePCs.Add(battlePC);
			battlePC->NotificationFromServer = FString("Welcome to BattleGround");
		}
		else battlePC->K2_DestroyActor();
	}
}

void ACBattleGM::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ACBattlePC* battlePC = Cast<ACBattlePC>(Exiting);

	if (battlePC)
		AlivePCs.Remove(battlePC);
}

void ACBattleGM::OnCharacterDeath(ACBattlePC * playerController)
{
	if (playerController)
	{
		playerController->ClientRPC_ShowDeathResult(AlivePCs.Num());
		AlivePCs.Remove(playerController);
		DeadPCs.Add(playerController);
	}
}

void ACBattleGM::AllDive()
{
	for (ACBattlePC* pc : AlivePCs)
	{
		if (pc)
		{
			if (!pc->bDived)
			{
				pc->ServerRPC_WantToDive();
			}
		}
	}
}

void ACBattleGM::MainTimer()
{
	if (PlayingState == EPlayingState::Waiting)
	{
		if (AlivePCs.Num() >= nMinimumPlayerCount)
		{
			nRemainTimeToStart -= 1;

			if (nRemainTimeToStart == 0)
			{
				PlayingState = EPlayingState::Battle;
				SayToAll("Battle Start!");

				for (ACBattlePC* pc : AlivePCs)
				{
					if(pc)
						pc->ServerRPC_RideInAirplaneC130(Airplane_C130);
				}

				Airplane_C130->bStarted = true;

				FTimerHandle timerHandle;
				GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ACBattleGM::ClearSayToAll, 5.0f, false);
			}
			else
			{
				FString text = FormatMinSec(nRemainTimeToStart) + FString("left before the battle begins");

				SayToAll(text);
			}
		}
		else
		{
			nRemainTimeToStart = nWaitingTime;
			SayToAll("Waiting for another user to connect");
		}
	}
	else if(PlayingState == EPlayingState::Battle)
	{
		GameState = Cast<ACBattleGS>(GetWorld()->GetGameState());

		if (GameState != NULL)
		{
			GameState->UpdateCircle_MainTimer();
			GameState->bDamageAvailable = true;
			GameState->nAliveCount = AlivePCs.Num();
		}

		if (AlivePCs.Num() <= 1)
		{
			if (AlivePCs[0])
			{
				AlivePCs[0]->ClientRPC_ShowWinnerResult();
				PlayingState = EPlayingState::Ceremony;
			}
		}
		else
		{
			if (GameState != NULL)
			{
				if (GameState->bVisibleCurrentCircle)
				{
					for(int i=0; i<AlivePCs.Num(); i++)
					{
						ACPlayer* player = Cast<ACPlayer>(AlivePCs[i]->GetPawn());

						if (player)
						{
							FVector playerPos = FVector(player->GetActorLocation().X, player->GetActorLocation().Y, 0.0f);
							FVector circlePos = FVector(GameState->CurrentCirclePosition.X, GameState->CurrentCirclePosition.Y, 0.0f);

							float Dist = FVector::Dist(playerPos, circlePos);

							if (Dist > GameState->CurrentCircleRadius)
							{
								player->ElectricDamage(10.0f);
							}
						}
					}
				}
			}
		}

	}
	else if(PlayingState == EPlayingState::Ceremony)
	{
		nRemainTimeOfCeremony -= 1;

		if (nRemainTimeOfCeremony <= 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(MainTimerHandle);

			for (ACBattlePC* pc : AlivePCs)
			{
				if (pc)
					pc->ClientRPC_BackToLobby();
			}

			for (ACBattlePC* pc : DeadPCs)
			{
				if (pc)
					pc->ClientRPC_BackToLobby();
			}

			UGameplayStatics::OpenLevel(GetWorld(), GetWorld()->GetCurrentLevel()->GetFName(), true, FString(TEXT("listen")));
		}
		else
		{
			SayToAll(FString::Printf(TEXT("After %d seconds, move to the lobby."), nRemainTimeOfCeremony));
		}
	}
}

void ACBattleGM::SayToAll(FString message)
{
	for (ACBattlePC* pc : AlivePCs)
	{
		if(pc)
			pc->NotificationFromServer = message;
	}

	for (ACBattlePC* pc : DeadPCs)
	{
		if (pc)
			pc->NotificationFromServer = message;
	}
}

void ACBattleGM::ClearSayToAll()
{
	for (ACBattlePC* pc : AlivePCs)
	{
		pc->NotificationFromServer = FString("");
	}
}

FString ACBattleGM::FormatMinSec(int sec)
{
	FString TotalText;
	FString MinText;
	FString SecText;

	if (sec / 60 == 0)
	{
	}
	else
	{
		MinText = FString::Printf(TEXT("%d Min "), sec / 60);
	}
	
	if (sec % 60 == 0)
	{
		
	}
	else
	{
		SecText = FString::Printf(TEXT("%d Sec "), sec % 60);

		TotalText = MinText + SecText;
	}

	return TotalText;
}
