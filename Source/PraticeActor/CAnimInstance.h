// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float fMoveForward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float fMoveRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float fLowerRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float fCurrentLowerRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float fControllerPitch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float fControllerRoll;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAiming;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bRifleEquiped;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCrounching;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bSprinting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bProne;

	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();
	
};
