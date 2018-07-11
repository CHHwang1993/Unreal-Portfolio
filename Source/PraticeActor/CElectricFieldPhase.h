// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Engine/DataTable.h"
#include "CElectricFieldPhase.generated.h"

/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCElectricFieldPhase : public UEngine
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FElectricPhase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		int Phase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		FVector CurrentCirclePosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		float CurrentCircleRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		FVector TargetCirclePosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		float TargetCircleRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		int WaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		int ShowTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		int ShrinkTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		bool UseRandomTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		float RandomTargetDistanceMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElectricFieldVariable")
		float RandomTargetDistanceMax;

};
