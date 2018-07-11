// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CAirplane_C130.generated.h"

class USoundBase;

UCLASS()
class PRATICEACTOR_API ACAirplane_C130 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACAirplane_C130();

	class USoundCue* Audio_C130_Cue;
	bool bStarted;
	FVector StartLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AirPlane")
		class UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Propeller")
		class UStaticMeshComponent* Propeller01;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Propeller")
		class UStaticMeshComponent* Propeller02;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Propeller")
		class UStaticMeshComponent* Propeller03;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Propeller")
		class UStaticMeshComponent* Propeller04;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
		class UAudioComponent* Audio_C130;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OffEngine();
	
	
};
