// Fill out your copyright notice in the Description page of Project Settings.

#include "CFlyingViewer.h"
#include "CInteraction_Widget.h"
#include "CBattlePC.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "Engine.h"


// Sets default values
ACFlyingViewer::ACFlyingViewer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bOnlyRelevantToOwner = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRootComp;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(this->RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 3000.0f;
	SpringArmComp->bDoCollisionTest = false;

	ViewCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCameraComp->SetupAttachment(SpringArmComp);
	ViewCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UClass> interactionWidget(TEXT("/Game/Characters/Female/HUD/WidgetBP_Interaction.WidgetBP_Interaction_C"));
	if (interactionWidget.Succeeded())
		TSubInteractionWidget = interactionWidget.Object;


	InteractionWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	InteractionWidgetComp->SetupAttachment(this->RootComponent);
	InteractionWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComp->SetDrawAtDesiredSize(true);
	InteractionWidgetComp->SetWidgetClass(TSubInteractionWidget);
	InteractionWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 730.0f));

}
// Called when the game starts or when spawned
void ACFlyingViewer::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		UCInteraction_Widget* widget = Cast<UCInteraction_Widget>(InteractionWidgetComp->GetUserWidgetObject());

		if (widget)
		{
			widget->FoundFieldItemName = FString(TEXT("Jump Out"));
		}
	}
	
	
}

// Called every frame
void ACFlyingViewer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACFlyingViewer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Interaction",EInputEvent::IE_Pressed, this, &ACFlyingViewer::Diving);

	InputComponent->BindAxis("Turn", this, &ACFlyingViewer::Turn);
	InputComponent->BindAxis("LookUp", this, &ACFlyingViewer::LookUp);
}

void ACFlyingViewer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ACFlyingViewer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ACFlyingViewer::Diving()
{
	ACBattlePC* pc =  Cast<ACBattlePC>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	pc->ServerRPC_WantToDive();
}


