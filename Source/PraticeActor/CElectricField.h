// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CElectricField.generated.h"

class ACBattleGS;
class UStaticMeshComponent;

UCLASS()
class PRATICEACTOR_API ACElectricField : public AActor
{
	GENERATED_BODY()

private:
	ACBattleGS * BattleGS;
	
public:	
	// Sets default values for this actor's properties
	ACElectricField();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMesh")
		UStaticMeshComponent* StaticMeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
