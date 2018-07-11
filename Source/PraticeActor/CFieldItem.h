// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CFieldItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UDataTable;

UCLASS()
class PRATICEACTOR_API ACFieldItem : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_LoadItemMesh)
		int ItemClassID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bRandom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int> RandomItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* MeshComp;
	UPROPERTY()
		UDataTable* ItemDataTable;

	
public:	
	// Sets default values for this actor's properties
	ACFieldItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
		virtual void OnRep_LoadItemMesh();

};
