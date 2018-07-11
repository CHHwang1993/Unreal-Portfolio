// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Engine/DataTable.h"
#include "CItemDataBase.generated.h"

class USkeletalMesh;
class UStaticMesh;

UENUM(BlueprintType)
enum class EItemEquipType : uint8
{
	Equip_Upper,
	Equip_Lower,
	Equip_Helmet,
	Equip_Shoes,
	Equip_Weapon
};

UCLASS()
class PRATICEACTOR_API UCItemDataBase : public UEngine
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FItemBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataBase")
		int ItemClassID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataBase")
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataBase")
		EItemEquipType ItemEquipType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataBase")
		USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataBase")
		UStaticMesh* StaticMesh;

};
