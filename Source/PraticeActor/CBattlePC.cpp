// Fill out your copyright notice in the Description page of Project Settings.

#include "CBattlePC.h"
#include "CBattleGM.h"
#include "CPlayer.h"
#include "CAirplane_C130.h"
#include "CFlyingViewer.h"
#include "CResult_Widget.h"
#include "CClientMain_Widget.h"
#include "CWorldMap_Widget.h"
#include "Animation/WidgetAnimation.h"
#include "UserWidget.h"
#include "UnrealNetwork.h"
#include "Engine.h"

ACBattlePC::ACBattlePC()
	:bDived(false), bWorldMap(false)
	, MapSize(816000.0f,816000.0f,0.0f)
{
	static ConstructorHelpers::FObjectFinder<UClass> clientWidget(TEXT("/Game/GameMode/Battle/UMG/WidgetBP_ClientMain.WidgetBP_ClientMain_C"));
	if (clientWidget.Object)
		TSubClientMainWidget = clientWidget.Object;

	static ConstructorHelpers::FObjectFinder<UClass> resultWidget(TEXT("/Game/GameMode/Battle/UMG/WidgetBP_Result.WidgetBP_Result_C"));
	if (resultWidget.Object)
		TSubResultWidget = resultWidget.Object;

	static ConstructorHelpers::FObjectFinder<UClass> worldMapWidget(TEXT("/Game/WorldMap/WidgetBP_WorldMap.WidgetBP_WorldMap_C"));
	if (worldMapWidget.Object)
		TSubWorldMapWidget = worldMapWidget.Object;
}

void ACBattlePC::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("C130"), outActors);

	if (outActors.Num() > 0)
	{
		Airplane_C130 = Cast<ACAirplane_C130>(outActors[0]);
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	
	if (!HasAuthority())
	{
		ClientMainWidget = CreateWidget<UCClientMain_Widget>(GetWorld(), TSubClientMainWidget);
		ResultWidget = CreateWidget<UCResult_Widget>(GetWorld(), TSubResultWidget);

		if (ClientMainWidget)
		{
			ClientMainWidget->AddToViewport();
		}
	}
	
}

void ACBattlePC::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("WorldMap", EInputEvent::IE_Pressed, this, &ACBattlePC::OnOpenWorldMap);
}


bool ACBattlePC::GetAirplane()
{
	ACFlyingViewer* viewer = Cast<ACFlyingViewer>(GetPawn());

	if (viewer)
	{
		return true;
	}
	else return false;
}

FVector ACBattlePC::GetPawnDirection()
{
	ACFlyingViewer* viewer = Cast<ACFlyingViewer>(GetPawn());

	if (viewer)
	{
		return Airplane_C130->GetActorForwardVector();
	}
	else
	{
		return GetPawn()->GetActorForwardVector();
	}

	return FVector::ZeroVector;
}

void ACBattlePC::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACBattlePC, NotificationFromServer);
}

void ACBattlePC::OnCharacterDeath()
{
	ACBattleGM* gameMode =  Cast<ACBattleGM>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode)
	{
		gameMode->OnCharacterDeath(this);
	}
	
}

void ACBattlePC::OnOpenWorldMap()
{
	if (WorldMapWidget == NULL)
	{
		WorldMapWidget = CreateWidget<UCWorldMap_Widget>(GetWorld(), TSubWorldMapWidget);
	}

	if (WorldMapWidget != NULL)
	{
		if (!bWorldMap)
		{
			bWorldMap = true;
			WorldMapWidget->AddToViewport();
		}
		else
		{
			bWorldMap = false;
			WorldMapWidget->RemoveFromParent();
		}
		
	}
}

FVector ACBattlePC::GetPawnPosition()
{
	if (GetPawn())
	{
		return GetPawn()->GetActorLocation();
	}

	return FVector(9999999999,9999999999,0);
}

bool ACBattlePC::ServerRPC_RideInAirplaneC130_Validate(ACAirplane_C130 * airplane)
{
	return true;
}

void ACBattlePC::ServerRPC_RideInAirplaneC130_Implementation(ACAirplane_C130 * airplane)
{
	if (airplane)
	{
		GetPawn()->Destroy();

		FActorSpawnParameters parameters;
		parameters.Owner = this;
		parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector location = airplane->GetActorLocation();
		ACFlyingViewer* viewer = GetWorld()->SpawnActor<ACFlyingViewer>(ACFlyingViewer::StaticClass(), location, FRotator::ZeroRotator, parameters);
		Possess(viewer);
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		viewer->AttachToActor(airplane, rules);
	}
		
}

bool ACBattlePC::ServerRPC_WantToDive_Validate()
{
	return true;
}

void ACBattlePC::ServerRPC_WantToDive_Implementation()
{
	if (bDived) return;

	bDived = true;

	FVector location = GetPawn()->GetActorLocation();
	location.Z -= 1000.0f;
	FActorSpawnParameters parameters;
	parameters.Owner = this;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACPlayer* player = GetWorld()->SpawnActor<ACPlayer>(ACPlayer::StaticClass(), location, FRotator::ZeroRotator, parameters);

	GetPawn()->Destroy();

	if (player)
	{
		player->SetDive(true);
		Possess(player);
	}
}

void ACBattlePC::ClientRPC_BackToLobby_Implementation()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Lobby"));
}

void ACBattlePC::ClientRPC_ShowDeathResult_Implementation(int rank)
{
	if (ResultWidget)
	{
		ResultWidget->ResultString = FString(TEXT("that's fine, that can happen.."));
		ResultWidget->RankString = FString("#") + FString::FromInt(rank);
		ResultWidget->AddToViewport();

		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(ResultWidget->GetCachedWidget());
		SetInputMode(Mode);
		bShowMouseCursor = true;
	}
}


void ACBattlePC::ClientRPC_ShowWinnerResult_Implementation()
{
	if (ResultWidget)
	{
		ResultWidget->ResultString = FString(TEXT("WINNER WINNER CHICKEN DINNER"));
		ResultWidget->RankString = FString("#") + FString::FromInt(1);
		ResultWidget->AddToViewport();

		FInputModeUIOnly Mode;
		Mode.SetWidgetToFocus(ResultWidget->GetCachedWidget());
		SetInputMode(Mode);
		bShowMouseCursor = true;
	}
}

void ACBattlePC::ClientRPC_ShowBloodScreenEffect_Implementation()
{
	if (ClientMainWidget)
	{
		if (ClientMainWidget->BloodEffectAnimation)
		{
			ClientMainWidget->PlayAnimation(ClientMainWidget->BloodEffectAnimation);
		}
	}
}
