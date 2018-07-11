// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CBattlePC.generated.h"

class UCClientMain_Widget;
class UCResult_Widget;
class UCWorldMap_Widget;
class ACAirplane_C130;

/**
 * 
 */
UCLASS()
class PRATICEACTOR_API ACBattlePC : public APlayerController
{
	GENERATED_BODY()

private:
	ACBattlePC();

	TSubclassOf<class UUserWidget> TSubClientMainWidget;
	TSubclassOf<class UUserWidget> TSubResultWidget;
	TSubclassOf<class UUserWidget> TSubWorldMapWidget;

	FVector MapSize;
	ACAirplane_C130* Airplane_C130;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
public:
	bool bDived;
	bool bWorldMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UMG")
		UCClientMain_Widget* ClientMainWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UMG")
		UCResult_Widget* ResultWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UMG")
		UCWorldMap_Widget* WorldMapWidget;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		FString NotificationFromServer;

	void OnCharacterDeath();
	void OnOpenWorldMap();
	FVector GetPawnPosition();
	FVector GetMapSize() {return MapSize;}
	bool GetAirplane();
	FVector GetPawnDirection();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_RideInAirplaneC130(ACAirplane_C130* airplane);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_WantToDive();

	UFUNCTION(Client, Reliable)
		void ClientRPC_BackToLobby();
	UFUNCTION(Client, Reliable)
		void ClientRPC_ShowDeathResult(int rank);
	UFUNCTION(Client, Reliable)
		void ClientRPC_ShowWinnerResult();
	UFUNCTION(Client, Reliable)
		void ClientRPC_ShowBloodScreenEffect();


	
	
};
