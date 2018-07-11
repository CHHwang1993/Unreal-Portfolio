// Fill out your copyright notice in the Description page of Project Settings.

#include "CPlayer.h"
#include "Engine.h"
#include "CBattlePC.h"
#include "CBattleGS.h"
#include "CFireShake.h"
#include "CFieldItem.h"
#include "CItemDataBase.h"
#include "CInteraction_Widget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimMontage.h"
#include "UserWidget.h"
#include "Curves/CurveFloat.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "UnrealNetwork.h"

// Sets default values
ACPlayer::ACPlayer()
	:bRifleEquiped(false), bAiming(false), bOwnedWeapon(false),bCurrentFalling(false),bPreviousFalling(false),bDiving(false),bGliding(false), bCrounching(false), bSprinting(false)
	, fMoveForwardAmount(0.0f), fMoveRightAmount(0.0f), fHP(100.0f),fMaxHP(100.0f)
	,UpperClassID(105),LowerClassID(106),ShoesClassID(109)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	///////////////////////////////////////////////////////////리플리케이트 옵션///////////////////////////////////////////////////////////
	bReplicates = true;
	bReplicateMovement = true;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////캐릭터값 조절///////////////////////////////////////////////////////////
	GetCharacterMovement()->JumpZVelocity = 300.0f;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////펀치 충돌 콜리전///////////////////////////////////////////////////////////
	LeftSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LeftCollision"));
	RightSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RightCollision"));
	LeftSphereCollision->SetupAttachment(GetMesh(), FName("WeaponLeft"));
	RightSphereCollision->SetupAttachment(GetMesh(), FName("WeaponRight"));
	LeftSphereCollision->SetSphereRadius(15.0f);
	RightSphereCollision->SetSphereRadius(15.0f);
	LeftSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::OnOverlapBegin_Punch);
	RightSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::OnOverlapBegin_Punch);
	LeftSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////스프링암 세팅///////////////////////////////////////////////////////////
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(this->RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 80.0f));
	SpringArmComp->TargetArmLength = 0.0f;
	SpringArmComp->ProbeSize = 100.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bInheritRoll = false;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////카메라 세팅///////////////////////////////////////////////////////////
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->SetRelativeLocation(FVector(-140.0f, 0, 0));
	CameraComp->FieldOfView = 70.0f;
	CameraComp->bUsePawnControlRotation = false;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////메쉬 세팅///////////////////////////////////////////////////////////
	TArray<FName> TagArray;
	TagArray.Add(FName("Body"));

	GetMesh()->SetSkeletalMesh(LoadAsset<USkeletalMesh>(TEXT("/Game/Characters/Female/Parts/SK_Female_Body_Head")));
	GetMesh()->SetAnimInstanceClass(LoadAsset<UClass>(TEXT("/Game/Characters/AnimBP_Player.AnimBP_Player_C")));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -85.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f,0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->ComponentTags = TagArray;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////웨폰 세팅///////////////////////////////////////////////////////////
	WeaponBackComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBackComp"));
	WeaponRightComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponRightComp"));
	WeaponBackComp->SetupAttachment(GetMesh(), FName(TEXT("WeaponBack")));
	WeaponRightComp->SetupAttachment(GetMesh(), FName(TEXT("WeaponRight")));

	WeaponBackComp->SetStaticMesh(NULL);
	WeaponRightComp->SetStaticMesh(NULL);

	WeaponBackComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponBackComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponBackComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);

	WeaponRightComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponRightComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponRightComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);

	WeaponRifle = LoadAsset<UStaticMesh>(TEXT("/Game/ArbitraryStudio/Weapons/SM_Rifle"));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////코스튬 셋팅///////////////////////////////////////////////////////////
	UpperComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Upper"));
	LowerComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Lower"));
	HelmetComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
	ShoesComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));

	UpperComp->SetupAttachment(GetMesh());
	LowerComp->SetupAttachment(GetMesh());
	HelmetComp->SetupAttachment(GetMesh());
	ShoesComp->SetupAttachment(GetMesh());

	UpperComp->SetMasterPoseComponent(GetMesh());
	LowerComp->SetMasterPoseComponent(GetMesh());
	HelmetComp->SetMasterPoseComponent(GetMesh());
	ShoesComp->SetMasterPoseComponent(GetMesh());

	HelmetComp->SetSkeletalMesh(LoadAsset<USkeletalMesh>(TEXT("/Game/Characters/Female/Parts/SK_Female_Cloth_Helmet_Hair")));
	UpperComp->SetSkeletalMesh(LoadAsset<USkeletalMesh>(TEXT("/Game/Characters/Female/Parts/SK_Female_Cloth_Upper_Tea")));
	LowerComp->SetSkeletalMesh(LoadAsset<USkeletalMesh>(TEXT("/Game/Characters/Female/Parts/SK_Female_Cloth_Lower_Leggings")));
	ShoesComp->SetSkeletalMesh(LoadAsset<USkeletalMesh>(TEXT("/Game/Characters/Female/Parts/SK_Female_Cloth_Shoes_Canvas")));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////애님 몽타주 세팅///////////////////////////////////////////////////////////
	StandToCrounchMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/StandToCrounchMontage"));
	CrounchToStandMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/CrouchToStandMontage"));
	StandToProneMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/StandToProneMontage"));
	ProneToStandMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/ProneToStandMontage"));
	CrounchToProneMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/CrounchToProneMontage"));
	ProneToCrounchMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/ProneToCrounchMontage"));
	FireMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/Fire_Montage"));
	CrounchFireMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/CrounchFireMontage"));
	ProneFireMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/ProneFireMontage"));
	JumpMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/Jump_Montage"));
	RifleJumpMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/RifleJump_Montage"));
	DeathMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/Death_Montage"));
	FlyingMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/Flying_Montage"));
	GlidingMontage = LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/Gliding_Montage"));
	PunchMontage.Add(LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/LeftPunchMontage")));
	PunchMontage.Add(LoadAsset<UAnimMontage>(TEXT("/Game/Characters/Female/Animations/Montage/RightPunchMontage")));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////FX 세팅///////////////////////////////////////////////////////////////////////
	FireFX = LoadAsset<UParticleSystem>(TEXT("/Game/WeaponEffects/P_AssaultRifle_MF"));
	HitFX = LoadAsset<UParticleSystem>(TEXT("/Game/WeaponEffects/P_AssaultRifle_IH"));
	BloodFX = LoadAsset<UParticleSystem>(TEXT("/Game/WeaponEffects/P_body_bullet_impact"));
	WindFX = LoadAsset<UParticleSystem>(TEXT("/Game/Vehicles/Parachute/Effects/P_Cloud"));

	WindComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParachuteWind"));
	WindComp->SetTemplate(WindFX);
	WindComp->SetupAttachment(SpringArmComp);
	WindComp->SetRelativeLocation(FVector(1700.0f, 0.0f, 0.0f));
	WindComp->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	WindComp->bAutoActivate = false;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////사운드 세팅///////////////////////////////////////////////////////////

	FireSound = LoadAsset<USoundBase>(TEXT("/Game/ArbitraryStudio/Weapons/rifle_sound"));
	PunchSound = LoadAsset<USoundBase>(TEXT("/Game/BGM/Punch"));
	WindCue = LoadAsset<USoundCue>(TEXT("/Game/Vehicles/Parachute/Effects/wind_Cue"));
	OpenParachuteCue = LoadAsset<USoundCue>(TEXT("/Game/Vehicles/Parachute/Effects/parachute_opening_Cue"));

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComp->SetSound(WindCue);
	AudioComp->bOverrideAttenuation = true;
	AudioComp->SetRelativeLocation(FVector(1400.0f, 0.0f, 0.0f));
	AudioComp->bAutoActivate = false;
	AudioComp->SetupAttachment(WindComp);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////위젯세팅///////////////////////////////////////////////////////////
	TSubCrossHairWidget = LoadAsset<UClass>(TEXT("/Game/Characters/Female/HUD/WidgetBP_CrossHair.WidgetBP_CrossHair_C"));
	TSubInteractionWidget = LoadAsset<UClass>(TEXT("/Game/Characters/Female/HUD/WidgetBP_Interaction.WidgetBP_Interaction_C"));

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(SpringArmComp);
	InteractionWidget->SetWidgetClass(TSubInteractionWidget);
	InteractionWidget->SetDrawAtDesiredSize(true);
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidget->SetPivot(FVector2D(0.0f, 0.5f));
	InteractionWidget->SetRelativeLocation(FVector(0.0f, 30.0f, -20.0f));
	InteractionWidget->bVisible = false;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////커브 및 FOV 세팅///////////////////////////////////////////////////////////

	//카메라FOV 타임라인
	CameraFOVTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));
	CameraFOVTimeLine->SetLooping(false);
	CameraFOVTimeLine->SetIgnoreTimeDilation(true);

	FloatCurve = LoadAsset<UCurveFloat>(TEXT("/Game/Timeline/FloatCurve"));
	SpringArmCurve = LoadAsset<UCurveFloat>(TEXT("/Game/Timeline/SpringArmCurve"));

	if (FloatCurve != NULL && SpringArmCurve!=NULL)
	{
		FOnTimelineFloat InterpFunction;
		FOnTimelineFloat MoveSpringArmLocation;
		InterpFunction.BindUFunction(this, FName("TimelineFloatReturn"));
		MoveSpringArmLocation.BindUFunction(this, FName("SpringArmLocationReturn"));
		CameraFOVTimeLine->AddInterpFloat(FloatCurve, InterpFunction, NAME_None,FName("FovChanged"));
		CameraFOVTimeLine->AddInterpFloat(SpringArmCurve, MoveSpringArmLocation, FName("SpringArmlocationChanged"));
	}

	//Pose 타임라인
	PoseTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrounchTimeLine"));
	PoseTimeLine->SetLooping(false);
	PoseTimeLine->SetIgnoreTimeDilation(true);

	CrounchCurve = LoadAsset<UCurveFloat>(TEXT("/Game/Timeline/CrounchCurve"));
	ProneCurve = LoadAsset<UCurveFloat>(TEXT("/Game/Timeline/ProneCurve"));
	ProneCrounchCurve = LoadAsset<UCurveFloat>(TEXT("/Game/Timeline/ProneCrounchCurve"));

	if (CrounchCurve != NULL)
	{
		FOnTimelineFloat InterpFunction;
		InterpFunction.BindUFunction(this, FName("PoseReturn"));
		PoseTimeLine->AddInterpFloat(CrounchCurve, InterpFunction, NAME_None,FName("PoseChanged"));
	}

	WaistTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("WaistTimeLine"));
	WaistTimeLine->SetLooping(false);
	WaistTimeLine->SetIgnoreTimeDilation(true);

	RightLeanCurve = LoadAsset<UCurveFloat>(TEXT("/Game/Timeline/RightLeanCurve"));
	LeftLeanCurve = LoadAsset<UCurveFloat>(TEXT("/Game/Timeline/LeftLeanCurve"));

	if (RightLeanCurve != NULL)
	{
		FOnTimelineFloat InterpFunction;
		InterpFunction.BindUFunction(this, FName("WaistReturn"));
		WaistTimeLine->AddInterpFloat(RightLeanCurve, InterpFunction, NAME_None, FName("WaistChanged"));
	}


	//랙돌 타임라인
	RagDollTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RagDollTimeline"));
	RagDollTimeline->SetLooping(false);
	RagDollTimeline->SetIgnoreTimeDilation(true);

	RagDollCurve = LoadAsset<UCurveFloat>(TEXT("/Game/Timeline/RagDollCurve"));

	if (RagDollCurve != NULL)
	{
		FOnTimelineFloat UpdateFunction;
		FOnTimelineEventStatic FinishFunction;
		UpdateFunction.BindUFunction(this, FName("RagDollReturn"));
		FinishFunction.BindUFunction(this, FName("RagDollFinished"));
		RagDollTimeline->AddInterpFloat(RagDollCurve, UpdateFunction, FName("RagDollChanged"));
		RagDollTimeline->SetTimelineFinishedFunc(FinishFunction);
	}

	//낙하산 타임라인
	ParachuteTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ParachuteTimeline"));
	ParachuteTimeline->SetLooping(false);
	ParachuteTimeline->SetIgnoreTimeDilation(true);

	ParachuteCurve = LoadAsset<UCurveFloat>(TEXT("/Game/Timeline/ParachuteCurve"));

	if (ParachuteCurve != NULL)
	{
		FOnTimelineFloat UpdateFunction;
		UpdateFunction.BindUFunction(this, FName("ParachuteOpeningAnim"));
		ParachuteTimeline->AddInterpFloat(ParachuteCurve, UpdateFunction, FName("ParachuteAnimation"));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////데이터 테이블 세팅///////////////////////////////////////////////////////////
	ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, TEXT("/Game/Item/ItemClassTable")));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////낙하산 세팅///////////////////////////////////////////////////////////
	ParachuteComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Parachute"));
	ParachuteComp->SetupAttachment(GetMesh());
	ParachuteComp->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	ParachuteComp->SetRelativeLocation(FVector(0.0f, 8.2f, 133.6f));
	ParachuteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParachuteComp->SetVisibility(false);

	ParachuteComp->SetStaticMesh(LoadAsset<UStaticMesh>(TEXT("/Game/Vehicles/Parachute/Meshes/Parachute")));

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 }

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	fHP = fMaxHP;

	if (isMyClient)
	{
		if (TSubCrossHairWidget != NULL)
		{
			CrossHairWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), TSubCrossHairWidget);
		}
	}
}

// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isMyClient)
	{
		if (bDiving || bGliding)
		{
			FFindFloorResult result;
			GetCharacterMovement()->ComputeFloorDist(GetCapsuleComponent()->GetAttachmentRootActor()->GetActorLocation(), 30000.0f, 30000.0f, result,GetCapsuleComponent()->GetScaledCapsuleRadius());

			if (result.FloorDist < 20000.0f)
			{
				if (result.FloorDist < 500.0f)
				{
					if(Role == ROLE_AutonomousProxy)
						ServerRPC_WantToWalk();
				}
				else
				{
					if (!bGliding)
					{
						if (Role == ROLE_AutonomousProxy)
							ServerRPC_WantToGlide();
					}
				}
				WindComp->Deactivate();
			}

			fCurrentTargetLocalPitch = FMath::FInterpTo(fCurrentTargetLocalPitch, fTargetLocalPitch, 0.3f, 0.05f);
			fCurrentTargetLocalRoll = FMath::FInterpTo(fCurrentTargetLocalRoll, fTargetLocalRoll, 0.3f, 0.05f);

			GetMesh()->SetRelativeRotation(FRotator(fCurrentTargetLocalRoll, -90.0f, fCurrentTargetLocalPitch));
		}
		else
		{
			ToggleInteractionWidget();
		}
	}
	//점프
	if (!bDiving && !bGliding)
	{
		bCurrentFalling = GetCharacterMovement()->IsFalling();
		BeginJump();
		Jumping();
		EndJump();
		bPreviousFalling = bCurrentFalling;
	}
	//
}

// Called to bind functionality to input
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Crounch", EInputEvent::IE_Pressed, this, &ACPlayer::SwitchCrounch);
	InputComponent->BindAction("Prone", EInputEvent::IE_Pressed, this, &ACPlayer::SwitchProne);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACPlayer::StartSprint);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACPlayer::StopSprint);
	InputComponent->BindAction("EquipRifle", EInputEvent::IE_Pressed, this, &ACPlayer::EquipRifle);
	InputComponent->BindAction("RightLean", EInputEvent::IE_Pressed, this, &ACPlayer::PlayRightLean);
	InputComponent->BindAction("RightLean", EInputEvent::IE_Released, this, &ACPlayer::ReverseRightLean);
	InputComponent->BindAction("LeftLean", EInputEvent::IE_Pressed, this, &ACPlayer::PlayLeftLean);
	InputComponent->BindAction("LeftLean", EInputEvent::IE_Released, this, &ACPlayer::ReverseLeftLean);
	InputComponent->BindAction("Aiming", EInputEvent::IE_Pressed, this, &ACPlayer::PlayAiming);
	InputComponent->BindAction("Aiming", EInputEvent::IE_Released, this, &ACPlayer::EndAiming);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ACPlayer::Fire);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::Jump);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACPlayer::StopJumping);
	InputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &ACPlayer::PickUpItem);
	InputComponent->BindAction("RotateCamera", EInputEvent::IE_Pressed, this, &ACPlayer::RotationCamera);
	InputComponent->BindAction("RotateCamera", EInputEvent::IE_Released, this, &ACPlayer::LockRotationCamera);

	InputComponent->BindAxis("MoveForward", this, &ACPlayer::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACPlayer::MoveRight);
	InputComponent->BindAxis("Turn", this, &ACPlayer::Turn);
	InputComponent->BindAxis("LookUp", this, &ACPlayer::LookUp);

}

void ACPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPlayer, fHP);
	DOREPLIFETIME(ACPlayer, fMoveForwardAmount);
	DOREPLIFETIME(ACPlayer, fMoveRightAmount);
	DOREPLIFETIME(ACPlayer, fWaistPitch);
	DOREPLIFETIME(ACPlayer, fWaistRoll);
	DOREPLIFETIME(ACPlayer, bCrounching);
	DOREPLIFETIME(ACPlayer, bProne);
	DOREPLIFETIME(ACPlayer, bSprinting);
	DOREPLIFETIME(ACPlayer, bRifleEquiped);
	DOREPLIFETIME(ACPlayer, bAiming);
	DOREPLIFETIME(ACPlayer, bDeath);
	DOREPLIFETIME(ACPlayer, bDiving);
	DOREPLIFETIME(ACPlayer, bGliding);
	DOREPLIFETIME(ACPlayer, UpperClassID);
	DOREPLIFETIME(ACPlayer, LowerClassID);
	DOREPLIFETIME(ACPlayer, HelmetClassID);
	DOREPLIFETIME(ACPlayer, ShoesClassID);
	DOREPLIFETIME(ACPlayer, WeaponClassID);
}

void ACPlayer::MoveForward(float value)
{
	//Prone 관련된 포즈일때는 몽타주 실행동안 멈춘다.
	if(bPlayingStandToProne) return;
	if(bPlayingProneToStand) return;
	if(bPlayingCrounchToProne) return;
	if (bPlayingProneToCrounch) return;

	fMoveForwardAmount = value;

	FVector vec = GetActorForwardVector();
	AddMovementInput(vec, fMoveForwardAmount);

	if (bSprinting && value>0)
		ServerRPC_SetCharacterMovement(ECharacterState::ESprint);
	else if(bSprinting && value <0)
		ServerRPC_SetCharacterMovement(ECharacterState::EWalk);


	if (fPrevForwardAmount != fMoveForwardAmount)
	{		
		ServerRPC_SetMoveForwardAmount(fMoveForwardAmount);
	}
	fPrevForwardAmount = fMoveForwardAmount;

	if(bGliding || bDiving) fTargetLocalPitch = fMoveForwardAmount * 20.0f;
}

void ACPlayer::MoveRight(float value)
{
	//Prone 관련된 포즈일때는 몽타주 실행동안 멈춘다.
	if (bPlayingStandToProne) return;
	if (bPlayingProneToStand) return;
	if (bPlayingCrounchToProne) return;
	if (bPlayingProneToCrounch) return;

	fMoveRightAmount = value;

	FVector vec = GetActorRightVector();
	AddMovementInput(vec, fMoveRightAmount);

	if (fPrevRightAmount != fMoveRightAmount)
	{
		ServerRPC_SetMoveRightAmount(fMoveRightAmount);
	}
	fPrevRightAmount = fMoveRightAmount;

	if (bGliding || bDiving) fTargetLocalRoll = fMoveRightAmount * 20.0f;
}

void ACPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ACPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);

	fWaistPitch = GetControlRotation().Pitch;

	if (abs(fWaistPitch - fPevWaistPitch) >1)
	{
		ServerRPC_WaistPitch(fWaistPitch);
	}

	fPevWaistPitch = fWaistPitch;
}

void ACPlayer::SwitchCrounch()
{
	if (Role == ROLE_AutonomousProxy)
	{
		if(bRifleEquiped && bOwnedWeapon)
		{
			if (!bAiming)
			{
				if (!bCrounching && !bProne) //스탠드 자세
				{
					ServerRPC_Crounch(true);

					if (StandToCrounchMontage)
						PlayAnimMontage(StandToCrounchMontage);

					if (PoseTimeLine)
					{
						PoseTimeLine->SetFloatCurve(CrounchCurve,FName("PoseChanged"));
						PoseTimeLine->PlayFromStart();
					}
				}
				else if (bCrounching) //앉아있는 자세
				{
					ServerRPC_Crounch(false);

					if (CrounchToStandMontage)
						PlayAnimMontage(CrounchToStandMontage);

					if (PoseTimeLine)
					{
						PoseTimeLine->SetFloatCurve(CrounchCurve, FName("PoseChanged"));
						PoseTimeLine->Reverse();
					}
				}
				else if (bProne)
				{
					ServerRPC_Prone(false);
					ServerRPC_Crounch(true);

					if (ProneToCrounchMontage)
						PlayAnimMontage(ProneToCrounchMontage);

					if (PoseTimeLine)
					{
						PoseTimeLine->SetFloatCurve(ProneCrounchCurve, FName("PoseChanged"));
						PoseTimeLine->Reverse();
					}
				}

				ServerRPC_CrounchAnimation();
			}
		}
	}
}

void ACPlayer::SwitchProne()
{
	if (Role == ROLE_AutonomousProxy)
	{
		if (bRifleEquiped && bOwnedWeapon)
		{
			if (!bAiming)
			{
				if (!bCrounching && !bProne) //스탠드 자세
				{
					ServerRPC_Prone(true);

					if (StandToProneMontage)
						PlayAnimMontage(StandToProneMontage);

					if (PoseTimeLine)
					{
						PoseTimeLine->SetFloatCurve(ProneCurve, FName("PoseChanged"));
						PoseTimeLine->PlayFromStart();
					}
				}
				else if (bCrounching) //앉아있는 자세
				{
					ServerRPC_Crounch(false);
					ServerRPC_Prone(true);

					if (CrounchToProneMontage)
						PlayAnimMontage(CrounchToProneMontage);

					if (PoseTimeLine)
					{
						PoseTimeLine->SetFloatCurve(ProneCrounchCurve, FName("PoseChanged"));
						PoseTimeLine->PlayFromStart();
					}
				}
				else if (bProne) //누워있는 자세
				{
					ServerRPC_Prone(false);

					if (ProneToStandMontage)
						PlayAnimMontage(ProneToStandMontage);

					if (PoseTimeLine)
					{
						PoseTimeLine->SetFloatCurve(ProneCurve, FName("PoseChanged"));
						PoseTimeLine->Reverse();
					}
				}
				
				ServerRPC_ProneAnimation();
			}
		}
	}
}

void ACPlayer::StartSprint()
{
	if (bCrounching || bProne) return;

	bSprinting = true;

	ServerRPC_Sprint(bSprinting);
}

void ACPlayer::StopSprint()
{
	if (bCrounching || bProne) return;

	bSprinting = false;

	ServerRPC_Sprint(bSprinting);

	ServerRPC_SetCharacterMovement(ECharacterState::EWalk);
}

void ACPlayer::PlayRightLean()
{
	if (WaistTimeLine)
	{
		WaistTimeLine->SetFloatCurve(RightLeanCurve, FName("WaistChanged"));
		WaistTimeLine->PlayFromStart();
	}
	
}

void ACPlayer::ReverseRightLean()
{
	if (WaistTimeLine)
	{
		WaistTimeLine->SetFloatCurve(RightLeanCurve, FName("WaistChanged"));
		WaistTimeLine->Reverse();
	}
}

void ACPlayer::PlayLeftLean()
{
	if (WaistTimeLine)
	{
		WaistTimeLine->SetFloatCurve(LeftLeanCurve, FName("WaistChanged"));
		WaistTimeLine->PlayFromStart();
	}
}

void ACPlayer::ReverseLeftLean()
{
	if (WaistTimeLine)
	{
		WaistTimeLine->SetFloatCurve(LeftLeanCurve, FName("WaistChanged"));
		WaistTimeLine->Reverse();
	}
}


void ACPlayer::EquipRifle()
{
	if (!bOwnedWeapon) return;

	if (Role == ROLE_AutonomousProxy)
	{
		if (!bRifleEquiped) ServerRPC_EquipRifle(true);
		else ServerRPC_EquipRifle(false);
	}
}

void ACPlayer::PlayAiming()
{
	if (!bOwnedWeapon) return;
	if (!bRifleEquiped) return;

	//Prone 관련된 포즈일때는 몽타주 실행동안 멈춘다.
	if (bPlayingStandToProne) return;
	if (bPlayingProneToStand) return;
	if (bPlayingCrounchToProne) return;
	if (bPlayingProneToCrounch) return;

	bAiming = true;

	ServerRPC_Aiming(bAiming);

	if (CrossHairWidget)
		CrossHairWidget->AddToViewport();

	if (CameraFOVTimeLine != NULL)
	{
		CameraFOVTimeLine->PlayFromStart();
	}
		
}

void ACPlayer::EndAiming()
{
	bAiming = false;

	ServerRPC_Aiming(bAiming);

	if (CrossHairWidget)
		CrossHairWidget->RemoveFromParent();

	if (CameraFOVTimeLine != NULL)
	{
		CameraFOVTimeLine->Reverse();
	}
}

void ACPlayer::Fire()
{
	if (bDiving || bGliding) return;

	if (bAiming && bRifleEquiped && bOwnedWeapon)
	{
		if (!bCrounching && !bProne && FireMontage)
			PlayAnimMontage(FireMontage);
		else if (bCrounching && CrounchFireMontage)
			PlayAnimMontage(CrounchFireMontage);
		else if (bProne && ProneFireMontage)
			PlayAnimMontage(ProneFireMontage);

		ServerRPC_FireAnimation();
	}
	else if (!bRifleEquiped)
	{
		if (PunchMontage[0] && PunchMontage[1])
		{
			if(!bPlayingLeftPunch && !bPlayingRightPunch)
				PlayAnimMontage(PunchMontage[UKismetMathLibrary::RandomIntegerInRange(0, PunchMontage.Num() - 1)]);
		}

		ServerRPC_FireAnimation();
	}
}

void ACPlayer::RotationCamera()
{
	if (bAiming) return;

	bUseControllerRotationYaw = false;

	ServerRPC_SetRotationYaw(false);
}

void ACPlayer::LockRotationCamera()
{
	if (bAiming) return;

	bUseControllerRotationYaw = true;

	ServerRPC_SetRotationYaw(true);

	GetController()->SetControlRotation(GetActorRotation());
}

void ACPlayer::BeginJump()
{
	if (bDiving || bGliding) return;

	if (bCurrentFalling && !bPreviousFalling)
	{
		if (bRifleEquiped)
		{
			PlayAnimMontage(RifleJumpMontage);
		}
		else
		{
			PlayAnimMontage(JumpMontage);
		}
	}
}

void ACPlayer::Jumping()
{
	if (bCurrentFalling && bPreviousFalling)
	{
		if (GetVelocity().Z < 0.0f)
		{

			FFindFloorResult floorResult;
			GetCharacterMovement()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(), 10000.0f, 10000.0f, floorResult, GetCapsuleComponent()->GetScaledCapsuleRadius());

			if (floorResult.FloorDist < 50.0f)
			{
				if (bRifleEquiped)
				{
					PlayAnimMontage(RifleJumpMontage, 1.0f, FName("Landing"));
				}
				else
				{
					PlayAnimMontage(JumpMontage, 1.0f, FName("Landing"));
				}
			}
		}
	}
}

void ACPlayer::EndJump()
{
	if (!bCurrentFalling && bPreviousFalling)
	{
		if (bRifleEquiped)
		{
			StopAnimMontage(RifleJumpMontage);
		}
		else
		{
			StopAnimMontage(JumpMontage);
		}
	}
}

void ACPlayer::ToggleInteractionWidget()
{
	//아이템찾기
	FindItemAroundMe();

	if (FoundFieldItem)
	{
		//데이터테이블의 아이템의 행을 가져온다.
		FName id = (*FString::FromInt(FoundFieldItem->ItemClassID));
		ItemBase = ItemDataTable->FindRow<FItemBase>(id, FString(""));

		if (ItemBase)
		{
			UCInteraction_Widget* widget_Interaction = Cast<UCInteraction_Widget>(InteractionWidget->GetUserWidgetObject());
			if (widget_Interaction)
				widget_Interaction->FoundFieldItemName = ItemBase->ItemName;

			InteractionWidget->bVisible = true;
		}
	}
	else InteractionWidget->bVisible = false;
}

ACFieldItem* ACPlayer::FindItemAroundMe()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
	objectType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);
	TArray<AActor*> OutActors;

	int Closest = -1;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), this->GetActorLocation(), 200, objectType, ACFieldItem::StaticClass(), ignoreActors, OutActors);

	if (OutActors.Num()>0)
	{
		int index = 0;
		
		for (AActor* actor : OutActors)
		{
			ACFieldItem* item = Cast<ACFieldItem>(actor);
			FHitResult hitResult;
			FVector EndLocation = (CameraComp->GetForwardVector()*1000.0f) + SpringArmComp->GetComponentLocation();
			FCollisionQueryParams TraceParams;
			bool hit = item->SphereCollision->LineTraceComponent(hitResult, SpringArmComp->GetComponentLocation(), EndLocation, TraceParams);

			if (hit)
			{
				Closest = index;
			}
			index++;
		}

		if (Closest == -1)
		{
			index = 0;
			float minDistance = 999999;
			for (AActor* actor : OutActors)
			{
				float dist;
				dist = FVector::Dist(actor->GetActorLocation(), this->GetActorLocation());

				if (dist < minDistance)
				{
					minDistance = dist;
					Closest = index;
				}
				index++;
			}
		}
	}
	else
	{
		//초기화
		FoundFieldItem = NULL;
		return FoundFieldItem;
	}


	if (Closest == -1) return NULL;

	ACFieldItem* item = Cast<ACFieldItem>(OutActors[Closest]);

	if (item != NULL)
	{
		FoundFieldItem = item;
		return FoundFieldItem;
	}
	else
		return NULL;
}

void ACPlayer::PickUpItem()
{
	if (bDiving)
	{
		ServerRPC_WantToGlide();
	}
	else
	{
		FindItemAroundMe();

		//ACFieldItem으로 형변환이 성공!
		if (FoundFieldItem)
		{
			if (Role == ROLE_AutonomousProxy)
			{
				ServerRPC_PickUpItem(FoundFieldItem);
			}
		}
	}
}

void ACPlayer::SwitchItem(ACFieldItem * item, int itemClassID)
{
	FActorSpawnParameters parameters;
	ACFieldItem* dropitem = Cast<ACFieldItem>(GetWorld()->SpawnActor<ACFieldItem>(ACFieldItem::StaticClass(), item->GetActorTransform(), parameters));
	if (dropitem)
	{
		dropitem->ItemClassID = itemClassID;
	}
}

void ACPlayer::ResetToWalk()
{
	ParachuteComp->SetVisibility(false);
	bUseControllerRotationYaw = true;
	ServerRPC_SetCharacterMovement(ECharacterState::EWalk);
	SpringArmComp->TargetArmLength = 300.0f;
	if (FlyingMontage)
		StopAnimMontage(FlyingMontage);
	if (GlidingMontage)
		StopAnimMontage(GlidingMontage);

	fCurrentTargetLocalPitch = 0.0f;
	fCurrentTargetLocalRoll = 0.0f;
	fTargetLocalPitch = 0.0f;
	fTargetLocalRoll = 0.0f;
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	AudioComp->Deactivate();
}

void ACPlayer::OnOverlapBegin_Punch_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	if (OtherActor != NULL)
	{
		ACPlayer* player = Cast<ACPlayer>(OtherActor);

		if (player)
		{
			TSubclassOf<UDamageType> TSubDamageType;

			if (PunchSound != NULL && (isMyClient || isOtherClient))
				UGameplayStatics::PlaySoundAtLocation(this, PunchSound, GetActorLocation());

			ServerRPC_ApplyPointDamage(player, 5, CameraComp->GetForwardVector(), SweepResult, GetController(), this, TSubDamageType);
		}
	}

}

void ACPlayer::OnShoot()
{
	if (isMyClient)
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(UCFireShake::StaticClass(), 1.0f);

	if (FireFX != NULL && (isMyClient || isOtherClient))
		UGameplayStatics::SpawnEmitterAttached(FireFX, WeaponRightComp, FName(TEXT("Muzzle")), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,true);

	if (FireSound != NULL && (isMyClient || isOtherClient))
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

	if (!isMyClient) return;

	FVector EndLocation = (CameraComp->GetForwardVector()*10000.0f) + SpringArmComp->GetComponentLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
	objectType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	objectType.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	objectType.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	objectType.Add(UEngineTypes::ConvertToObjectType(ECC_Vehicle));
	objectType.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);

	FHitResult hitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(this, SpringArmComp->GetComponentLocation(), EndLocation, objectType, true, ignoreActors, EDrawDebugTrace::None, hitResult, true);

	if (hitResult.bBlockingHit)
	{
		AActor* actor = Cast<ACPlayer>(hitResult.Actor.Get());

		if (actor && hitResult.GetComponent()->ComponentHasTag(FName("Body")))
		{
			FVector fxLocation = hitResult.ImpactPoint + (hitResult.ImpactNormal*20.0f);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodFX, fxLocation, FRotator::ZeroRotator, FVector(2.0f, 2.0f, 2.0f), true);

			TSubclassOf<UDamageType> TSubDamageType;

			ServerRPC_ApplyPointDamage(actor, 10, CameraComp->GetForwardVector(), hitResult, GetController(), this, TSubDamageType);
			ServerRPC_ShowAttackEffect(true, fxLocation);
		}
		else
		{
			FVector fxLocation = hitResult.ImpactPoint + (hitResult.ImpactNormal*20.0f);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, fxLocation);

			ServerRPC_ShowAttackEffect(false, fxLocation);
		}
	}
}

void ACPlayer::OnSetLeftCollisionEnable(bool isbool)
{
	if (isbool)
		LeftSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		LeftSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACPlayer::OnSetRightCollisionEnable(bool isbool)
{
	if (isbool)
		RightSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		RightSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	FPointDamageEvent const* const pointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

	ACBattleGS* gs = Cast<ACBattleGS>(UGameplayStatics::GetGameState(GetWorld()));
	
	if (gs)
	{
		if (gs->bDamageAvailable)
		{
			if (pointDamageEvent->HitInfo.BoneName.ToString() == "Head")
			{
				if (HelmetClassID == 103)
				{
					fHP -= 30.0f;
				}
				else
				{
					fHP -= 100.0f;
				}
			}
				
			else
				this->fHP -= DamageAmount;


			if (fHP <= 0)
			{
				MulticastRPC_DeathAnimation();

				bDeath = true;

				ACBattlePC* pc = Cast<ACBattlePC>(GetController());
				if (pc)
					pc->OnCharacterDeath();
			}
			else
			{
				MulticastRPC_BeingAttackedAnimation();

			}
		}
	}
	
	return Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
}

void ACPlayer::ElectricDamage(float damage)
{
	fHP -= damage;

	if (fHP <= 0)
	{
		fHP = 0.0f;
		MulticastRPC_DeathAnimation();

		bDeath = true;

		ACBattlePC* pc = Cast<ACBattlePC>(GetController());
		if (pc)
			pc->OnCharacterDeath();
	}
	else
	{
		ACBattlePC* pc = Cast<ACBattlePC>(GetController());

		if (pc)
		{
			pc->ClientRPC_ShowBloodScreenEffect();
		}
	}
		

}

void ACPlayer::TimelineFloatReturn(float value)
{
	if (CameraComp)
		CameraComp->SetFieldOfView(value);
}

void ACPlayer::SpringArmLocationReturn(float value)
{
	fAimiingSpringArmY = value;

	if (SpringArmComp)
	{
		if (fWaistRoll >= 0)
		{
			if (!bCrounching && !bProne) SpringArmComp->SetRelativeLocation(FVector(0.0f, fAimiingSpringArmY + fWaistRoll * 0.5f, 80.0f));
			else if (bCrounching) SpringArmComp->SetRelativeLocation(FVector(0.0f, fAimiingSpringArmY + fWaistRoll * 0.5f, 0.0f));
			else if (bProne)  SpringArmComp->SetRelativeLocation(FVector(0.0f, fAimiingSpringArmY + fWaistRoll * 0.5f, -40.0f));
		}
		else
		{
			if (!bCrounching && !bProne) SpringArmComp->SetRelativeLocation(FVector(0.0f, -fAimiingSpringArmY + fWaistRoll * 0.5f, 80.0f));
			else if (bCrounching) SpringArmComp->SetRelativeLocation(FVector(0.0f, -fAimiingSpringArmY + fWaistRoll * 0.5f, 0.0f));
			else if (bProne)  SpringArmComp->SetRelativeLocation(FVector(0.0f, -fAimiingSpringArmY + fWaistRoll * 0.5f, -40.0f));
		}
	}
}

void ACPlayer::PoseReturn(float value)
{
	if(SpringArmComp)
		SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, value));
}

void ACPlayer::WaistReturn(float value)
{
	fWaistRoll = value;

	if (SpringArmComp)
	{
		if (fWaistRoll >= 0)
		{
			if (!bCrounching && !bProne) SpringArmComp->SetRelativeLocation(FVector(0.0f, fAimiingSpringArmY + value * 0.5, 80.0f));
			else if (bCrounching) SpringArmComp->SetRelativeLocation(FVector(0.0f, fAimiingSpringArmY + value * 0.5, 0.0f));
			else if (bProne)  SpringArmComp->SetRelativeLocation(FVector(0.0f, fAimiingSpringArmY + value * 0.5, -40.0f));
		}
		else if (fWaistRoll < 0 && fAimiingSpringArmY >0)
		{
			if (!bCrounching && !bProne) SpringArmComp->SetRelativeLocation(FVector(0.0f, -fAimiingSpringArmY + value * 0.5, 80.0f));
			else if (bCrounching) SpringArmComp->SetRelativeLocation(FVector(0.0f, -fAimiingSpringArmY + value * 0.5, 0.0f));
			else if (bProne)  SpringArmComp->SetRelativeLocation(FVector(0.0f, -fAimiingSpringArmY + value * 0.5, -40.0f));
		}
		else if (fWaistRoll < 0 && fAimiingSpringArmY < 0)
		{
			if (!bCrounching && !bProne) SpringArmComp->SetRelativeLocation(FVector(0.0f, fAimiingSpringArmY + value * 0.5, 80.0f));
			else if (bCrounching) SpringArmComp->SetRelativeLocation(FVector(0.0f, fAimiingSpringArmY + value * 0.5, 0.0f));
			else if (bProne)  SpringArmComp->SetRelativeLocation(FVector(0.0f, fAimiingSpringArmY + value * 0.5, -40.0f));
		}
	}
}

void ACPlayer::RagDollReturn(float value)
{
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName("Spine"), value);
}

void ACPlayer::RagDollFinished()
{
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("Spine"), false);
}

void ACPlayer::ParachuteOpeningAnim(float value)
{
	ParachuteComp->SetRelativeScale3D(FVector(value, value, value));
}


void ACPlayer::SetDeferredAsyncLoadHelmet()
{
	TAssetPtr<USkeletalMesh> skeletalMesh(HelmetAsset);
	HelmetComp->SetSkeletalMesh(skeletalMesh.Get());
}

void ACPlayer::SetDeferredAsyncLoadUpper()
{
	TAssetPtr<USkeletalMesh> skeletalMesh(UpperAsset);
	UpperComp->SetSkeletalMesh(skeletalMesh.Get());
}

void ACPlayer::SetDeferredAsyncLoadLower()
{
	TAssetPtr<USkeletalMesh> skeletalMesh(LowerAsset);
	LowerComp->SetSkeletalMesh(skeletalMesh.Get());
}

void ACPlayer::SetDeferredAsyncLoadShoes()
{
	TAssetPtr<USkeletalMesh> skeletalMesh(ShoesAsset);
	ShoesComp->SetSkeletalMesh(skeletalMesh.Get());
}

void ACPlayer::SetDeferredAsyncLoadWeapon()
{
	TAssetPtr<UStaticMesh> staticMesh(WeaponAsset);
	WeaponRifle = staticMesh.Get();
	bOwnedWeapon = true;

	if (!bRifleEquiped)
	{
		WeaponBackComp->SetStaticMesh(WeaponRifle);
		WeaponRightComp->SetStaticMesh(NULL);
	}
	else
	{
		WeaponBackComp->SetStaticMesh(NULL);
		WeaponRightComp->SetStaticMesh(WeaponRifle);
	}
}



/////////////////////////////////////서버///////////////////////////////////////////////////

bool ACPlayer::ServerRPC_SetMoveForwardAmount_Validate(float forwardAmount)
{
	return true;
}

void ACPlayer::ServerRPC_SetMoveForwardAmount_Implementation(float forwardAmount)
{
	fMoveForwardAmount = forwardAmount;
}

bool ACPlayer::ServerRPC_SetMoveRightAmount_Validate(float rightAmount)
{
	return true;
}

void ACPlayer::ServerRPC_SetMoveRightAmount_Implementation(float rightAmount)
{
	fMoveRightAmount = rightAmount;
}

bool ACPlayer::ServerRPC_SetRotationYaw_Validate(bool isYaw)
{
	return true;
}

void ACPlayer::ServerRPC_SetRotationYaw_Implementation(bool isYaw)
{
	this->bUseControllerRotationYaw = isYaw;
}

bool ACPlayer::ServerRPC_Sprint_Validate(bool sprint)
{
	return true;
}

void ACPlayer::ServerRPC_Sprint_Implementation(bool sprint)
{
	bSprinting = sprint;
}

bool ACPlayer::ServerRPC_Crounch_Validate(bool crounch)
{
	return true;
}

void ACPlayer::ServerRPC_Crounch_Implementation(bool crounch)
{
	bCrounching = crounch;

	if (bCrounching)
		MulticastRPC_SetMaxWalkSpeed(250.0f);
	else
		MulticastRPC_SetMaxWalkSpeed(400.0f);
}

bool ACPlayer::ServerRPC_Prone_Validate(bool prone)
{
	return true;
}

void ACPlayer::ServerRPC_Prone_Implementation(bool prone)
{
	bProne = prone;

	if (bProne)
		MulticastRPC_SetMaxWalkSpeed(100.0f);
	else
		MulticastRPC_SetMaxWalkSpeed(400.0f);
}


bool ACPlayer::ServerRPC_EquipRifle_Validate(bool equiped)
{
	return true;
}

void ACPlayer::ServerRPC_EquipRifle_Implementation(bool equiped)
{
	bRifleEquiped = equiped;
}

void ACPlayer::OnRep_EquipRifle()
{
	if (bRifleEquiped)
	{
		WeaponBackComp->SetStaticMesh(NULL);
		WeaponRightComp->SetStaticMesh(WeaponRifle);
	}
	else
	{
		WeaponBackComp->SetStaticMesh(WeaponRifle);
		WeaponRightComp->SetStaticMesh(NULL);
	}
}
bool ACPlayer::ServerRPC_Aiming_Validate(bool aiming)
{
	return true;
}

void ACPlayer::ServerRPC_Aiming_Implementation(bool aiming)
{
	bAiming = aiming;
}

bool ACPlayer::ServerRPC_PickUpItem_Validate(ACFieldItem* item)
{
	return true;
}

void ACPlayer::ServerRPC_PickUpItem_Implementation(ACFieldItem* item)
{
	if (item == NULL) return;
	if (item->IsActorBeingDestroyed() == true) return;

	FName id = FName(*FString::FromInt(item->ItemClassID));

	ItemBase = ItemDataTable->FindRow<FItemBase>(id, FString(""));

	if (ItemBase != NULL)
	{
		if (ItemBase->ItemEquipType == EItemEquipType::Equip_Helmet)
		{
			if (HelmetClassID != 0) SwitchItem(item, HelmetClassID);

			HelmetClassID = item->ItemClassID;
		}
		else if (ItemBase->ItemEquipType == EItemEquipType::Equip_Upper)
		{
			if (UpperClassID != 0) SwitchItem(item, UpperClassID);

			UpperClassID = item->ItemClassID;
		}
		else if (ItemBase->ItemEquipType == EItemEquipType::Equip_Lower)
		{
			if (LowerClassID != 0) SwitchItem(item, LowerClassID);

			LowerClassID = item->ItemClassID;
		}
		else if (ItemBase->ItemEquipType == EItemEquipType::Equip_Shoes)
		{
			if (ShoesClassID != 0) SwitchItem(item, ShoesClassID);

			ShoesClassID = item->ItemClassID;
		}
		else if (ItemBase->ItemEquipType == EItemEquipType::Equip_Weapon)
		{
			if (WeaponClassID != 0) SwitchItem(item, WeaponClassID);

			WeaponClassID = item->ItemClassID;
		}
	}

	item->K2_DestroyActor();
}

void ACPlayer::MulticastRPC_PickUpItem_Implementation(int itemClassID)
{
	if (itemClassID == 0) return;

	FName id = FName(*FString::FromInt(itemClassID));

	ItemBase = ItemDataTable->FindRow<FItemBase>(id, FString(""));

	if (ItemBase != NULL)
	{
		if (ItemBase->ItemEquipType == EItemEquipType::Equip_Helmet) HelmetClassID = itemClassID;
		else if (ItemBase->ItemEquipType == EItemEquipType::Equip_Upper) UpperClassID = itemClassID;
		else if (ItemBase->ItemEquipType == EItemEquipType::Equip_Lower) LowerClassID = itemClassID;
		else if (ItemBase->ItemEquipType == EItemEquipType::Equip_Shoes) ShoesClassID = itemClassID;
		else if (ItemBase->ItemEquipType == EItemEquipType::Equip_Weapon) WeaponClassID = itemClassID;
	}
}

void ACPlayer::OnRep_SetUpperClassID()
{
	if (UpperClassID == 0) return;

	FName id = FName(*FString::FromInt(UpperClassID));

	ItemBase = ItemDataTable->FindRow<FItemBase>(id, FString(""));

	if (ItemBase)
	{
		UpperAsset = ItemBase->SkeletalMesh;
		UAssetManager::GetStreamableManager().RequestAsyncLoad(UpperAsset,FStreamableDelegate::CreateUObject(this,&ACPlayer::SetDeferredAsyncLoadUpper));
	}
}

void ACPlayer::OnRep_SetLowerClassID()
{
	if (LowerClassID == 0) return;

	FName id = FName(*FString::FromInt(LowerClassID));

	ItemBase = ItemDataTable->FindRow<FItemBase>(id, FString(""));

	if (ItemBase)
	{
		LowerAsset = ItemBase->SkeletalMesh;
		UAssetManager::GetStreamableManager().RequestAsyncLoad(LowerAsset, FStreamableDelegate::CreateUObject(this, &ACPlayer::SetDeferredAsyncLoadLower));
	}
}

void ACPlayer::OnRep_SetHelmetClassID()
{
	if (HelmetClassID == 0) return;

	FName id = FName(*FString::FromInt(HelmetClassID));

	ItemBase = ItemDataTable->FindRow<FItemBase>(id, FString(""));

	if (ItemBase)
	{
		HelmetAsset = ItemBase->SkeletalMesh;
		UAssetManager::GetStreamableManager().RequestAsyncLoad(HelmetAsset, FStreamableDelegate::CreateUObject(this, &ACPlayer::SetDeferredAsyncLoadHelmet));
	}
}

void ACPlayer::OnRep_SetShoesClassID()
{
	if (ShoesClassID == 0) return;

	FName id = FName(*FString::FromInt(ShoesClassID));

	ItemBase = ItemDataTable->FindRow<FItemBase>(id, FString(""));

	if (ItemBase)
	{
		ShoesAsset = ItemBase->SkeletalMesh;
		UAssetManager::GetStreamableManager().RequestAsyncLoad(ShoesAsset, FStreamableDelegate::CreateUObject(this, &ACPlayer::SetDeferredAsyncLoadShoes));
	}
}

void ACPlayer::OnRep_SetWeaponClassID()
{
	if (WeaponClassID == 0) return;

	FName id = FName(*FString::FromInt(WeaponClassID));

	ItemBase = ItemDataTable->FindRow<FItemBase>(id, FString(""));

	if (ItemBase)
	{
		WeaponAsset = ItemBase->StaticMesh;
		UAssetManager::GetStreamableManager().RequestAsyncLoad(WeaponAsset, FStreamableDelegate::CreateUObject(this, &ACPlayer::SetDeferredAsyncLoadWeapon));
	}
}

void ACPlayer::OnRep_Death()
{
	if (bDeath)
	{
		PlayAnimMontage(DeathMontage);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

void ACPlayer::OnRep_Diving()
{
	if (!bDiving && !bGliding)
	{
		ResetToWalk();
	}
	if (bDiving)
	{
		PlayAnimMontage(FlyingMontage);
		bUseControllerRotationYaw = false;
		ServerRPC_SetCharacterMovement(ECharacterState::EDiving);
		AudioComp->Activate(true);
		WindComp->Activate(true);
	}
}

void ACPlayer::OnRep_Gliding()
{
	if (!bGliding && !bDiving)
	{
		ResetToWalk();
	}
	if (bGliding)
	{
		PlayAnimMontage(GlidingMontage);

		if (OpenParachuteCue)
			UGameplayStatics::PlaySound2D(GetWorld(), OpenParachuteCue);

		if (ParachuteTimeline)
			ParachuteTimeline->PlayFromStart();

		ParachuteComp->SetVisibility(true);
		bUseControllerRotationYaw = false;
		ServerRPC_SetCharacterMovement(ECharacterState::EGliding);
		SpringArmComp->TargetArmLength = 700.0f;
	}
}

bool ACPlayer::ServerRPC_FireAnimation_Validate()
{
	return true;
}

void ACPlayer::ServerRPC_FireAnimation_Implementation()
{
	MulticastRPC_FireAnimation();
}

void ACPlayer::MulticastRPC_FireAnimation_Implementation()
{
	if (isOtherClient)
	{
		if (bRifleEquiped)
		{
			if (!bCrounching && !bProne)
				PlayAnimMontage(FireMontage);
			else if (bCrounching)
				PlayAnimMontage(CrounchFireMontage);
			else if (bProne)
				PlayAnimMontage(ProneFireMontage);
		}
		else if (!bRifleEquiped)
		{
			if (PunchMontage[0] && PunchMontage[1])
			{
				if (!bPlayingLeftPunch && !bPlayingRightPunch)
					PlayAnimMontage(PunchMontage[UKismetMathLibrary::RandomIntegerInRange(0, PunchMontage.Num() - 1)]);
			}
		}
	}
}

bool ACPlayer::ServerRPC_CrounchAnimation_Validate()
{
	return true;
}

void ACPlayer::ServerRPC_CrounchAnimation_Implementation()
{
	MulticastRPC_CrounchAnimation();
}

void ACPlayer::MulticastRPC_CrounchAnimation_Implementation()
{
	if (isOtherClient)
	{
		if (!bCrounching && !bProne) //스탠드 자세
		{
			if (StandToCrounchMontage)
				PlayAnimMontage(StandToCrounchMontage);
		}
		else if (bCrounching) //앉아있는 자세
		{
			if (CrounchToStandMontage)
				PlayAnimMontage(CrounchToStandMontage);
		}
		else if (bProne)
		{
			if (ProneToCrounchMontage)
				PlayAnimMontage(ProneToCrounchMontage);
		}
	}
}

bool ACPlayer::ServerRPC_ProneAnimation_Validate()
{
	return true;
}

void ACPlayer::ServerRPC_ProneAnimation_Implementation()
{
	MulticastRPC_ProneAnimation();
}

void ACPlayer::MulticastRPC_ProneAnimation_Implementation()
{
	if (isOtherClient)
	{
		if (!bCrounching && !bProne) //스탠드 자세
		{
			if (StandToProneMontage)
				PlayAnimMontage(StandToProneMontage);
		}
		else if (bCrounching) //앉아있는 자세
		{
			if (CrounchToProneMontage)
				PlayAnimMontage(CrounchToProneMontage);
		}
		else if (bProne) //누워있는 자세
		{
			if (ProneToStandMontage)
				PlayAnimMontage(ProneToStandMontage);
		}
	}
}

bool ACPlayer::ServerRPC_ShowAttackEffect_Validate(bool isHuman, FVector spawnLocation)
{
	return true;
}

void ACPlayer::ServerRPC_ShowAttackEffect_Implementation(bool isHuman, FVector spawnLocation)
{
	MulticastRPC_ShowAttackEffect(isHuman,spawnLocation);
}

void ACPlayer::MulticastRPC_ShowAttackEffect_Implementation(bool isHuman, FVector spawnLocation)
{
	if (isOtherClient)
	{
		if(isHuman==true)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodFX, spawnLocation, FRotator::ZeroRotator, FVector(2.0f, 2.0f, 2.0f), true);
		else
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, spawnLocation);
	}
}

bool ACPlayer::ServerRPC_ApplyPointDamage_Validate(AActor* DamagedActor, float BaseDamage, FVector const& HitFromDirection, FHitResult const& HitInfo, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	return true;
}

void ACPlayer::ServerRPC_ApplyPointDamage_Implementation(AActor* DamagedActor, float BaseDamage, FVector const& HitFromDirection, FHitResult const& HitInfo, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	UGameplayStatics::ApplyPointDamage(DamagedActor, BaseDamage, HitFromDirection, HitInfo, EventInstigator, DamageCauser, DamageTypeClass);
}

void ACPlayer::MulticastRPC_DeathAnimation_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	WeaponBackComp->SetSimulatePhysics(true);
	WeaponRightComp->SetSimulatePhysics(true);

	WeaponBackComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	WeaponRightComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	if (RagDollTimeline)
		RagDollTimeline->Stop();
}

void ACPlayer::MulticastRPC_BeingAttackedAnimation_Implementation()
{
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("Spine"), true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName("Spine"), 1.0f);

	if (RagDollTimeline)
		RagDollTimeline->PlayFromStart();
}

bool ACPlayer::ServerRPC_WaistPitch_Validate(float value)
{
	return true;
}

void ACPlayer::ServerRPC_WaistPitch_Implementation(float value)
{
	fWaistPitch = value;
}

bool ACPlayer::ServerRPC_WantToGlide_Validate()
{
	return true;
}

void ACPlayer::ServerRPC_WantToGlide_Implementation()
{
	bDiving = false;
	bGliding = true;
}

bool ACPlayer::ServerRPC_WantToWalk_Validate()
{
	return true;
}

void ACPlayer::ServerRPC_WantToWalk_Implementation()
{
	bDiving = false;
	bGliding = false;
}

bool ACPlayer::ServerRPC_SetCharacterMovement_Validate(ECharacterState state)
{
	return true;
}

void ACPlayer::ServerRPC_SetCharacterMovement_Implementation(ECharacterState state)
{
	if (GetCharacterMovement() != NULL)
	{
		if (state == ECharacterState::EDiving)
		{
			GetCharacterMovement()->AirControl = 1.0f;
			GetCharacterMovement()->MaxWalkSpeed = 10000.0f;
			GetCharacterMovement()->GravityScale = 0.1f;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 10.0f, 0.0f);
		}
		else if (state == ECharacterState::EGliding)
		{
			GetCharacterMovement()->AirControl = 1.0f;
			GetCharacterMovement()->MaxWalkSpeed = 4000.0f;
			GetCharacterMovement()->GravityScale = 0.1f;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 10.0f, 0.0f);
			GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity*0.5f;
		}
		else if (state == ECharacterState::EWalk)
		{
			GetCharacterMovement()->AirControl = 0.05f;
			MulticastRPC_SetMaxWalkSpeed(400.0f);
			GetCharacterMovement()->GravityScale = 1.0f;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 360.0f);
		}
		else if (state == ECharacterState::ESprint)
		{
			MulticastRPC_SetMaxWalkSpeed(600.0f);
		}
	}
}

void ACPlayer::MulticastRPC_SetMaxWalkSpeed_Implementation(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}