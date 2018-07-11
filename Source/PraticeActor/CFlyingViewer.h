// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CFlyingViewer.generated.h"

UCLASS()
class PRATICEACTOR_API ACFlyingViewer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACFlyingViewer();

	TSubclassOf<class UUserWidget> TSubInteractionWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SceneRoot")
		class USceneComponent* SceneRootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm")
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* ViewCameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		class UWidgetComponent* InteractionWidgetComp;

private:
	void Turn(float value);
	void LookUp(float value);
	void Diving();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
