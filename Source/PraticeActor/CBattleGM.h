// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CBattleGM.generated.h"

enum class EPlayingState : uint8
{
	Waiting,
	Battle,
	Ceremony
};

class ACBattlePC;
class ACBattleGS;
class ACAirplane_C130;
/**
 * 
 */
UCLASS(Config=Game)
class PRATICEACTOR_API ACBattleGM : public AGameModeBase
{
	GENERATED_BODY()

private:
	TArray<ACBattlePC*> AlivePCs;
	TArray<ACBattlePC*> DeadPCs;
	FTimerHandle MainTimerHandle;
	EPlayingState PlayingState;
	ACBattleGS* GameState;
	ACAirplane_C130* Airplane_C130;
	int nRemainTimeToStart;
	UPROPERTY(Config)
		int nMinimumPlayerCount;
	UPROPERTY(Config)	
		int nWaitingTime;
	UPROPERTY(Config)
		int nRemainTimeOfCeremony;

private:
	ACBattleGM();

	void MainTimer();
	FString FormatMinSec(int sec);

protected:
	virtual void BeginPlay() override;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void OnCharacterDeath(ACBattlePC* playerController);
	void AllDive();
	void SayToAll(FString message);
	void ClearSayToAll();

	ACBattleGS* GetBattleGameState() { return GameState; }
};
