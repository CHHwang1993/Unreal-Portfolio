// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

#define isServer (HasAuthority())
#define isMyClient ((!HasAuthority()) && (GetController()==UGameplayStatics::GetPlayerController(GetWorld(),0)))
#define isOtherClient ((!HasAuthority()) && (GetController()!=UGameplayStatics::GetPlayerController(GetWorld(),0)))

UENUM()
enum class ECharacterState : uint8
{
	EDiving,
	EGliding,
	EWalk,
	ESprint
};

class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UParticleSystemComponent;
class UWidgetComponent;
class UTimelineComponent;
class UAudioComponent;
class UAnimMontage;
class USoundBase;
class UParticleSystem;
class UCurveFloat;
class USoundCue;
class ACFieldItem;
class UDataTable;
class UUserWidget;

struct FItemBase;


UCLASS()
class PRATICEACTOR_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

private:

	bool bOwnedWeapon;
	bool bCurrentFalling;
	bool bPreviousFalling;
	bool bPlayingStandToProne;
	bool bPlayingProneToStand;
	bool bPlayingCrounchToProne;
	bool bPlayingProneToCrounch;
	bool bPlayingLeftPunch;
	bool bPlayingRightPunch;
	float fPrevForwardAmount;
	float fPrevRightAmount;
	float fPevWaistPitch;
	float fMaxHP;
	float fTargetLocalPitch;
	float fTargetLocalRoll;
	float fCurrentTargetLocalPitch;
	float fCurrentTargetLocalRoll;
	float fAimiingSpringArmY;

	UStaticMesh * WeaponRifle;

	TSubclassOf<class UUserWidget> TSubCrossHairWidget;
	TSubclassOf<class UUserWidget> TSubInteractionWidget;

	UCurveFloat* FloatCurve;
	UCurveFloat* SpringArmCurve;
	UCurveFloat* CrounchCurve;
	UCurveFloat* ProneCurve;
	UCurveFloat* ProneCrounchCurve;
	UCurveFloat* RightLeanCurve;
	UCurveFloat* LeftLeanCurve;
	UCurveFloat* RagDollCurve;
	UCurveFloat* ParachuteCurve;

	FStringAssetReference HelmetAsset;
	FStringAssetReference UpperAsset;
	FStringAssetReference LowerAsset;
	FStringAssetReference ShoesAsset;
	FStringAssetReference WeaponAsset;
	ACFieldItem* FoundFieldItem;
	FItemBase* ItemBase;
	UDataTable* ItemDataTable;

public:
	UPROPERTY(Replicated)
		float fHP;
	UPROPERTY(Replicated)
		float fMoveForwardAmount;
	UPROPERTY(Replicated)
		float fMoveRightAmount;
	UPROPERTY(Replicated)
		float fWaistRoll;
	UPROPERTY(Replicated)
		float fWaistPitch;
	UPROPERTY(Replicated)
		bool bCrounching;
	UPROPERTY(Replicated)
		bool bProne;
	UPROPERTY(Replicated)
		bool bAiming;
	UPROPERTY(Replicated)
		bool bSprinting;

	
	UPROPERTY(ReplicatedUsing = OnRep_SetUpperClassID)
		int UpperClassID;
	UPROPERTY(ReplicatedUsing = OnRep_SetLowerClassID)
		int LowerClassID;
	UPROPERTY(ReplicatedUsing = OnRep_SetHelmetClassID)
		int HelmetClassID;
	UPROPERTY(ReplicatedUsing = OnRep_SetShoesClassID)
		int ShoesClassID;
	UPROPERTY(ReplicatedUsing = OnRep_SetWeaponClassID)
		int WeaponClassID;
	UPROPERTY(ReplicatedUsing = OnRep_EquipRifle)
		bool bRifleEquiped;
	UPROPERTY(ReplicatedUsing = OnRep_Death)
		bool bDeath;
	UPROPERTY(ReplicatedUsing = OnRep_Diving)
		bool bDiving;
	UPROPERTY(ReplicatedUsing = OnRep_Gliding)
		bool bGliding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
		USphereComponent* LeftSphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
		USphereComponent* RightSphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
		UWidgetComponent* InteractionWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FX)
		UParticleSystemComponent* WindComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
		UAudioComponent* AudioComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
		UStaticMeshComponent* WeaponBackComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
		UStaticMeshComponent* WeaponRightComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Costume)
		USkeletalMeshComponent* UpperComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Costume)
		USkeletalMeshComponent* LowerComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Costume)
		USkeletalMeshComponent* HelmetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Costume)
		USkeletalMeshComponent* ShoesComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parachute)
		UStaticMeshComponent* ParachuteComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline)
		UTimelineComponent* CameraFOVTimeLine;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline)
		UTimelineComponent* PoseTimeLine;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline)
		UTimelineComponent* WaistTimeLine;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline)
		UTimelineComponent* RagDollTimeline;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timeline)
		UTimelineComponent* ParachuteTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
		UUserWidget* CrossHairWidget;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* StandToCrounchMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* CrounchToStandMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* StandToProneMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* ProneToStandMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* CrounchToProneMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* ProneToCrounchMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		TArray<UAnimMontage*> PunchMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* FireMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* CrounchFireMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* ProneFireMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* JumpMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* RifleJumpMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* FlyingMontage;

	UPROPERTY(VisibleAnywhere, Category = Animation)
		UAnimMontage* GlidingMontage;

	UPROPERTY(VisibleAnywhere, Category = Sound)
		USoundBase* PunchSound;

	UPROPERTY(VisibleAnywhere, Category = Sound)
		USoundBase* FireSound;

	UPROPERTY(VisibleAnywhere, Category = Sound)
		USoundCue* WindCue;

	UPROPERTY(VisibleAnywhere, Category = Sound)
		USoundCue* OpenParachuteCue;

	UPROPERTY(VisibleAnywhere, Category = FX)
		UParticleSystem* FireFX;

	UPROPERTY(VisibleAnywhere, Category = FX)
		UParticleSystem* HitFX;

	UPROPERTY(VisibleAnywhere, Category = FX)
		UParticleSystem* BloodFX;

	UPROPERTY(VisibleAnywhere, Category = FX)
		UParticleSystem* WindFX;

private:
	ACPlayer();

	template <class T>
	T* LoadAsset(const TCHAR* assetPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(assetPath);
		if (asset.Succeeded())
			return asset.Object;
		else
			return NULL;
	}

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);

	void SwitchCrounch();
	void SwitchProne();
	void StartSprint();
	void StopSprint();

	void PlayRightLean();
	void ReverseRightLean();
	void PlayLeftLean();
	void ReverseLeftLean();

	void BeginJump();
	void Jumping();
	void EndJump();

	void EquipRifle();
	void PlayAiming();
	void EndAiming();
	void Fire();

	void RotationCamera();
	void LockRotationCamera();

	void ToggleInteractionWidget();
	ACFieldItem* FindItemAroundMe();
	void PickUpItem();
	void SwitchItem(ACFieldItem* item, int itemClassID);

	void ResetToWalk();

	void SetDeferredAsyncLoadHelmet();
	void SetDeferredAsyncLoadUpper();
	void SetDeferredAsyncLoadLower();
	void SetDeferredAsyncLoadShoes();
	void SetDeferredAsyncLoadWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void ElectricDamage(float damage);

	float GetHP() { return fHP; }
	float GetMaxHP() { return fMaxHP; }
	float GetMoveForwardAmount() { return fMoveForwardAmount; }
	float GetMoveRightAmount() { return fMoveRightAmount; }
	float GetWaistRoll() { return fWaistRoll; }
	float GetWaistPitch() { return fWaistPitch; }
	bool GetAiming() { return bAiming; }
	bool GetRifleEquiped() { return bRifleEquiped; }
	bool GetCrounching() { return bCrounching; }
	bool GetProne() { return bProne; }
	bool GetSprinting() { return bSprinting; }
	UUserWidget* GetCrossWidget() { return CrossHairWidget; }

	void SetDive(bool dive) { bDiving = dive; }
	void SetPlayingStandToProne(bool isbool) { bPlayingStandToProne = isbool; }
	void SetPlayingProneToStand(bool isbool) { bPlayingProneToStand = isbool; }
	void SetPlayingCrounchToProne(bool isbool) { bPlayingCrounchToProne = isbool; }
	void SetPlayingProneToCrounch(bool isbool) { bPlayingProneToCrounch = isbool; }
	void SetPlayingLeftPunch(bool isbool) { bPlayingLeftPunch = isbool; }
	void SetPlayingRightPunch(bool isbool) { bPlayingRightPunch = isbool; }

	UFUNCTION(BlueprintNativeEvent)
		void OnOverlapBegin_Punch(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnShoot();	

	UFUNCTION(BlueprintCallable)
		void OnSetLeftCollisionEnable(bool isbool);
	UFUNCTION(BlueprintCallable)
		void OnSetRightCollisionEnable(bool isbool);


	UFUNCTION()
		void TimelineFloatReturn(float value);

	UFUNCTION()
		void SpringArmLocationReturn(float value);

	UFUNCTION()
		void PoseReturn(float value);

	UFUNCTION()
		void WaistReturn(float value);

	UFUNCTION()
		void RagDollReturn(float value);

	UFUNCTION()
		void RagDollFinished();

	UFUNCTION()
		void ParachuteOpeningAnim(float value);

	UFUNCTION()
		virtual void OnRep_EquipRifle();
	UFUNCTION()
		virtual void OnRep_SetUpperClassID();
	UFUNCTION()
		virtual void OnRep_SetLowerClassID();
	UFUNCTION()
		virtual void OnRep_SetHelmetClassID();
	UFUNCTION()
		virtual void OnRep_SetShoesClassID();
	UFUNCTION()
		virtual void OnRep_SetWeaponClassID();
	UFUNCTION()
		virtual void OnRep_Death();
	UFUNCTION()
		virtual void OnRep_Diving();
	UFUNCTION()
		virtual void OnRep_Gliding();
	
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_SetMoveForwardAmount(float forwardAmount);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_SetMoveRightAmount(float rightAmount);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_SetRotationYaw(bool isYaw);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_Sprint(bool sprint);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_Crounch(bool crounch);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_Prone(bool prone);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_EquipRifle(bool equiped);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_Aiming(bool aiming);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_PickUpItem(ACFieldItem* item);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPC_PickUpItem(int itemClassID);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_FireAnimation();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPC_FireAnimation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_CrounchAnimation();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPC_CrounchAnimation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_ProneAnimation();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPC_ProneAnimation();


	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_ShowAttackEffect(bool isHuman,FVector spawnLocation);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPC_ShowAttackEffect(bool isHuman, FVector spawnLocation);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_ApplyPointDamage(AActor* DamagedActor, float BaseDamage, FVector const& HitFromDirection, FHitResult const& HitInfo, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPC_DeathAnimation();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPC_BeingAttackedAnimation();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_WaistPitch(float value);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_WantToGlide();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_WantToWalk();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRPC_SetCharacterMovement(ECharacterState state);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRPC_SetMaxWalkSpeed(float speed);
};
