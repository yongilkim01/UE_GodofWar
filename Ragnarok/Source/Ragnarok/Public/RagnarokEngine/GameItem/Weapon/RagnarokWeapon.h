// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RagnarokWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UItemPrimaryAssetKratosWeapon;

UCLASS()
class RAGNAROK_API ARagnarokWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARagnarokWeapon();

public:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

protected:
	virtual void LoadWeaponDataAsset();
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Data")
	UItemPrimaryAssetKratosWeapon* WeaponPDA = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Data")
	FPrimaryAssetId WeaponPDAId;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
	UBoxComponent* WeaponCollision = nullptr;

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
};
