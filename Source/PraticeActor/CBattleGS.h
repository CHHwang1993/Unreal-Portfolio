// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CBattleGS.generated.h"

class UDataTable;

struct FElectricPhase;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API ACBattleGS : public AGameStateBase
{
	GENERATED_BODY()

private:
	ACBattleGS();

	int nWaitingTime;
	int nShowTime;
	int nShrinkTime;
	int PhaseNum;
	float fDeltaRadiusPerSec;
	bool bNotifiedShowTime;
	bool bNotifiedShrinkTime;
	FVector DeltaPositionPerSec;
	UDataTable* PhaseTable;
	FElectricPhase* CurrentPhase;
	

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(Replicated)
		int nAliveCount;
	UPROPERTY(Replicated)
		bool bDamageAvailable;
	UPROPERTY(Replicated)
		FVector CurrentCirclePosition;
	UPROPERTY(Replicated)
		FVector TargetCirclePosition;
	UPROPERTY(Replicated)
		float CurrentCircleRadius;
	UPROPERTY(Replicated)
		float TargetCircleRadius;
	UPROPERTY(Replicated)
		bool bVisibleCurrentCircle;
	UPROPERTY(Replicated)
		bool bVisibleTargetCircle;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateCircle_MainTimer();
	void SetElectricFieldPhase(int phase,FVector previousPosition, float previousRadius);
	void NotifyMessage(FString message);
	void ClearMessage();
};
