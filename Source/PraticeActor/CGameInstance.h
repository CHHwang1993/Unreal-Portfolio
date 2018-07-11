// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	FString IPAddress;
	
private:
	UCGameInstance();
	
	
};
