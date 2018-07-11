// Fill out your copyright notice in the Description page of Project Settings.

#include "CBattleGS.h"
#include "CBattleGM.h"
#include "Engine/DataTable.h"
#include "Engine.h"
#include "CElectricFieldPhase.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

ACBattleGS::ACBattleGS()
	:bDamageAvailable(false), bVisibleCurrentCircle(false), bVisibleTargetCircle(false), bNotifiedShowTime(false), bNotifiedShrinkTime(false)
	,CurrentCircleRadius(600000.0f),TargetCircleRadius(15000.0f)
	, TargetCirclePosition(11130.0f,312440.0f,0.0f), CurrentCirclePosition(0.0f, 0.0f, 0.0f)
	, nWaitingTime(60.0f), nShowTime(30.0f), nShrinkTime(10),nAliveCount(0), PhaseNum(1)
{
	bReplicates = true;
	bAlwaysRelevant = true;

	PhaseTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, TEXT("/Game/EletricField/ElectricFieldPhaseTable.ElectricFieldPhaseTable")));
}

void ACBattleGS::BeginPlay()
{
	Super::BeginPlay();

	SetElectricFieldPhase(PhaseNum,FVector::ZeroVector,0.0f);
}

void ACBattleGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACBattleGS, nAliveCount);
	DOREPLIFETIME(ACBattleGS, bDamageAvailable);
	DOREPLIFETIME(ACBattleGS, CurrentCirclePosition);
	DOREPLIFETIME(ACBattleGS, TargetCirclePosition);
	DOREPLIFETIME(ACBattleGS, CurrentCircleRadius);
	DOREPLIFETIME(ACBattleGS, TargetCircleRadius);
	DOREPLIFETIME(ACBattleGS, bVisibleCurrentCircle);
	DOREPLIFETIME(ACBattleGS, bVisibleTargetCircle);
}

void ACBattleGS::UpdateCircle_MainTimer()
{
	nWaitingTime -= 1;

	if(nWaitingTime < 0)
	{
		if (!bVisibleTargetCircle)
			bVisibleTargetCircle = true;
		else
		{
			nShowTime -= 1;

			if(nShowTime<0)
			{
				if (!bVisibleCurrentCircle)
				{
					bVisibleCurrentCircle = true;
				}
				else
				{
					nShrinkTime -= 1;

					if (nShrinkTime >= 0)
					{
						CurrentCirclePosition += DeltaPositionPerSec;
						CurrentCircleRadius += fDeltaRadiusPerSec;

						if (!bNotifiedShrinkTime)
						{
							NotifyMessage(FString("The Electric Field starts to shrink"));
							bNotifiedShrinkTime = true;
						}
					}
					else
					{
						if (bVisibleTargetCircle)
							bVisibleTargetCircle = false;

						PhaseNum += 1;

						if (PhaseNum < 4)
						{
							SetElectricFieldPhase(PhaseNum,TargetCirclePosition, TargetCircleRadius);
						}
					}
				}
			}
			else
			{
				if (!bNotifiedShowTime)
				{
					NotifyMessage(FString("A new Electric Field has been set"));
					bNotifiedShowTime = true;
				}
			}
		}
			
	}
}

void ACBattleGS::SetElectricFieldPhase(int phase, FVector previousPosition, float previousRadius)
{
	if (PhaseTable)
	{
		FName rowName = *FString::FromInt(phase);
		CurrentPhase = PhaseTable->FindRow<FElectricPhase>(rowName, FString(""));

		bNotifiedShowTime = false;
		bNotifiedShrinkTime = false;
		
		if (CurrentPhase)
		{
			CurrentCirclePosition = CurrentPhase->CurrentCirclePosition;
			CurrentCircleRadius = CurrentPhase->CurrentCircleRadius;
			TargetCirclePosition = CurrentPhase->TargetCirclePosition;
			TargetCircleRadius = CurrentPhase->TargetCircleRadius;

			nWaitingTime = CurrentPhase->WaitTime;
			nShowTime = CurrentPhase->ShowTime;
			nShrinkTime = CurrentPhase->ShrinkTime;

			bVisibleCurrentCircle = false;
			bVisibleTargetCircle = false;
			if (phase > 1) bVisibleCurrentCircle = true;

			if (CurrentPhase->UseRandomTarget)
			{
				CurrentCirclePosition = previousPosition;
				CurrentCircleRadius = previousRadius;

				TargetCirclePosition = CurrentCirclePosition + (UKismetMathLibrary::RandomUnitVector()*
					UKismetMathLibrary::RandomFloatInRange(CurrentPhase->RandomTargetDistanceMin, CurrentPhase->RandomTargetDistanceMax));
			}

			DeltaPositionPerSec = (TargetCirclePosition - CurrentCirclePosition) / nShrinkTime;
			fDeltaRadiusPerSec = (TargetCircleRadius - CurrentCircleRadius) / nShrinkTime;

		}
	}
}

void ACBattleGS::NotifyMessage(FString message)
{
	ACBattleGM* gm = Cast<ACBattleGM>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gm)
	{
		gm->SayToAll(message);

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ACBattleGS::ClearMessage, 5.0f, false);
	}
}

void ACBattleGS::ClearMessage()
{
	ACBattleGM* gm = Cast<ACBattleGM>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gm)
		gm->ClearSayToAll();
}
