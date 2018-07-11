// Fill out your copyright notice in the Description page of Project Settings.

#include "CFieldItem.h"
#include "CItemDataBase.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealNetwork.h"

// Sets default values
ACFieldItem::ACFieldItem()
	:bRandom(false), ItemClassID(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	this->RootComponent = SphereCollision; 

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	MeshComp->SetupAttachment(SphereCollision);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, TEXT("/Game/Item/ItemClassTable.ItemClassTable")));

}

// Called when the game starts or when spawned
void ACFieldItem::BeginPlay()
{
	Super::BeginPlay();

	if (bRandom)
	{
		if (HasAuthority())
		{
			if (RandomItems.Num() >= 1)
			{
				int index = UKismetMathLibrary::RandomIntegerInRange(0, RandomItems.Num() - 1);
				ItemClassID = RandomItems[index];
			}
		}
	}
	else
	{
		if (ItemClassID != 0)
		{
			FItemBase* row = ItemDataTable->FindRow<FItemBase>(*FString::Printf(TEXT("%d"), ItemClassID), FString(""));

			if (row != NULL)
			{
				if (row->ItemEquipType == EItemEquipType::Equip_Weapon) MeshComp->SetRelativeRotation(FQuat(FRotator(0.0f, 0.0f, 90.0f)));
				MeshComp->SetStaticMesh(row->StaticMesh);
				float radius;
				FVector origin, boxExtent;
				UKismetSystemLibrary::GetComponentBounds(MeshComp, origin, boxExtent, radius);
				SphereCollision->SetSphereRadius(radius);
				MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -radius * 0.5f));
			}
		}
	}

	
}

// Called every frame
void ACFieldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACFieldItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACFieldItem, ItemClassID);
}

void ACFieldItem::OnRep_LoadItemMesh()
{
	if (ItemClassID != 0)
	{
		FItemBase* row = ItemDataTable->FindRow<FItemBase>(*FString::Printf(TEXT("%d"), ItemClassID), FString(""));

		if (row != NULL)
		{
			if (row->ItemEquipType == EItemEquipType::Equip_Weapon) MeshComp->SetRelativeRotation(FQuat(FRotator(0.0f, 0.0f, 90.0f)));
			MeshComp->SetStaticMesh(row->StaticMesh);
			float radius;
			FVector origin, boxExtent;
			UKismetSystemLibrary::GetComponentBounds(MeshComp, origin, boxExtent, radius);
			SphereCollision->SetSphereRadius(radius);
			MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -radius * 0.5f));
		}
	}
}

