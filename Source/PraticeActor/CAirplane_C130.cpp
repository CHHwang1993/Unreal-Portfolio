// Fill out your copyright notice in the Description page of Project Settings.

#include "CAirplane_C130.h"
#include "CBattleGM.h"
#include "Engine.h"

// Sets default values
ACAirplane_C130::ACAirplane_C130()
	:bStarted(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;

	//비행기 몸체 세팅
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(this->RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> airplane(TEXT("/Game/Vehicles/C-130/Meshes/SM_C_130.SM_C_130"));
	if (airplane.Succeeded())
		Body->SetStaticMesh(airplane.Object);

	Body->SetRelativeLocation(FVector(-700.0f, 0.0f, -200.0f));
	Body->SetRelativeRotation(FRotator(0,90.0f,0.0f));
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//프로펠러 세팅
	static ConstructorHelpers::FObjectFinder<UStaticMesh> propeller(TEXT("/Game/Vehicles/C-130/Meshes/SM_C_130_Propellar.SM_C_130_Propellar"));
	if (!propeller.Succeeded()) return;

	Propeller01 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("P1"));
	Propeller02 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("P2"));
	Propeller03 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("P3"));
	Propeller04 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("P4"));

	Propeller01->SetStaticMesh(propeller.Object);
	Propeller02->SetStaticMesh(propeller.Object);
	Propeller03->SetStaticMesh(propeller.Object);
	Propeller04->SetStaticMesh(propeller.Object);

	Propeller01->SetupAttachment(Body);
	Propeller02->SetupAttachment(Body);
	Propeller03->SetupAttachment(Body);
	Propeller04->SetupAttachment(Body);

	Propeller01->SetRelativeLocation(FVector(-1012.0f, 238.0f, 404.0f));
	Propeller02->SetRelativeLocation(FVector(-504.0f, 238.0f, 373.0f));
	Propeller03->SetRelativeLocation(FVector(516.0f, 238.0f, 373.0f));
	Propeller04->SetRelativeLocation(FVector(1022.0f, 238.0f, 404.0f));

	//캐릭터 무브먼트 세팅
	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	GetCharacterMovement()->DefaultWaterMovementMode = EMovementMode::MOVE_Flying;
	GetCharacterMovement()->MaxFlySpeed = 72000.0f;

	//사운드세팅
	Audio_C130 = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio_C130->SetupAttachment(this->RootComponent);
	
	static ConstructorHelpers::FObjectFinder<USoundCue> c130_Cue(TEXT("/Game/Vehicles/C-130/Sounds/C130_Cue.C130_Cue"));
	if (c130_Cue.Succeeded())
		Audio_C130_Cue = c130_Cue.Object;

	if (Audio_C130)
	{
		Audio_C130->SetSound(Audio_C130_Cue);
		Audio_C130->bOverrideAttenuation = true;
	}
}

// Called when the game starts or when spawned
void ACAirplane_C130::BeginPlay()
{
	Super::BeginPlay();
	
	if(Audio_C130)
		Audio_C130->Play();
	StartLocation = GetActorLocation();
}

// Called every frame
void ACAirplane_C130::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (bStarted)
		{
			GetCharacterMovement()->AddInputVector(GetActorForwardVector());

			if (FVector::Dist(StartLocation, GetActorLocation()) > 700000)
			{
				ACBattleGM* gm = Cast<ACBattleGM>(UGameplayStatics::GetGameMode(GetWorld()));
				if (gm)
				{
					gm->AllDive();
					FTimerHandle timerHandle;
					GetWorld()->GetTimerManager().SetTimer(timerHandle,this,&ACAirplane_C130::OffEngine, 5.0f, false);
				}
			}
		}
	}
	else
	{
		Propeller01->AddLocalRotation(FRotator(DeltaTime*-720.0f, 0.0f, 0.0f));
		Propeller02->AddLocalRotation(FRotator(DeltaTime*-720.0f, 0.0f, 0.0f));
		Propeller03->AddLocalRotation(FRotator(DeltaTime*-720.0f, 0.0f, 0.0f));
		Propeller04->AddLocalRotation(FRotator(DeltaTime*-720.0f, 0.0f, 0.0f));
	}
}

// Called to bind functionality to input
void ACAirplane_C130::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACAirplane_C130::OffEngine()
{
	bStarted = false;
	PrimaryActorTick.bCanEverTick = false;
}

