// Fill out your copyright notice in the Description page of Project Settings.

#include "CAnimInstance.h"
#include "CPlayer.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInstance::UpdateAnimationProperties()
{
	ACPlayer* player = Cast<ACPlayer>(TryGetPawnOwner());

	if (player)
	{
		bAiming = player->GetAiming();
		bRifleEquiped = player->GetRifleEquiped();
		bCrounching = player->GetCrounching();
		bSprinting = player->GetSprinting();
		bProne = player->GetProne();
		fMoveForward = player->GetMoveForwardAmount();
		fMoveRight = player->GetMoveRightAmount();
		fLowerRotation = fMoveRight * 30.0f;
		fControllerRoll = player->GetWaistRoll();

		if (bAiming && !bProne)
			fControllerPitch = player->GetWaistPitch();
		else
			fControllerPitch = 0.0f;

		if (fMoveRight != 0 && fMoveForward == 0)
			fMoveForward = 1.0f;

		fLowerRotation = fMoveForward * fLowerRotation;

		fCurrentLowerRotation = UKismetMathLibrary::FInterpTo(fCurrentLowerRotation, fLowerRotation, 1.0f, 0.1f);

		if(player->ProneToStandMontage)
			player->SetPlayingProneToStand(Montage_IsPlaying(player->ProneToStandMontage));
		if (player->StandToProneMontage)
			player->SetPlayingStandToProne(Montage_IsPlaying(player->StandToProneMontage));

		if (player->CrounchToProneMontage)
			player->SetPlayingCrounchToProne(Montage_IsPlaying(player->CrounchToProneMontage));
		if (player->ProneToCrounchMontage)
			player->SetPlayingProneToCrounch(Montage_IsPlaying(player->ProneToCrounchMontage));
		if (player->PunchMontage[0])
			player->SetPlayingLeftPunch(Montage_IsPlaying(player->PunchMontage[0]));
		if (player->PunchMontage[1])
			player->SetPlayingRightPunch(Montage_IsPlaying(player->PunchMontage[1]));

	}
}

