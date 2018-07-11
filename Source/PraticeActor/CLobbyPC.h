// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CLobbyPC.generated.h"

class UCLobby_Widget;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API ACLobbyPC : public APlayerController
{
	GENERATED_BODY()

private:
	ACLobbyPC();

	class USoundCue* BattleCue;
	TSubclassOf<class UUserWidget> TSubLobbyWidget;
	UCLobby_Widget* LobbyWidget;

protected:
	virtual void BeginPlay() override;
	
public:
		void JoinServer(FString ipAdress);
};
