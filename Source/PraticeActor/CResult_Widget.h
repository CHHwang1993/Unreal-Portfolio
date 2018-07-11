// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CResult_Widget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class PRATICEACTOR_API UCResult_Widget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	FString ResultString;
	FString RankString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* ResultText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* RankText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* LobbyButton;

	UFUNCTION()
		void OnGoToLobby();
	
};
