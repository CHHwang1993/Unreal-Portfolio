// Fill out your copyright notice in the Description page of Project Settings.

#include "CElectricField.h"
#include "CBattleGS.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"

// Sets default values
ACElectricField::ACElectricField()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> electricCylinder(TEXT("/Game/EletricField/SM_EletricCylinder.SM_EletricCylinder"));
	if (electricCylinder.Succeeded())
		StaticMeshComp->SetStaticMesh(electricCylinder.Object);
}

// Called when the game starts or when spawned
void ACElectricField::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACElectricField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BattleGS = Cast<ACBattleGS>(UGameplayStatics::GetGameState(GetWorld()));

	if (BattleGS)
	{
		StaticMeshComp->SetVisibility(BattleGS->bVisibleCurrentCircle, true);
		this->SetActorLocation(BattleGS->CurrentCirclePosition);

		float scale = BattleGS->CurrentCircleRadius / 100.0f*2.0f;
		this->SetActorScale3D(FVector(scale, scale, 1000.0f));
	}
	

}

