// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/GameItem/Weapon/RagnarokWeapon.h"
#include "KratosWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API AKratosWeapon : public ARagnarokWeapon
{
	GENERATED_BODY()

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	//~ Begin ARagnarokWeapon Interface.
	virtual void LoadWeaponDataAsset() override;
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject) override;
	//~ End ARagnarokWeapon Interface.

protected:
	UItemPrimaryAssetKratosWeapon* KratosWeaponPDA = nullptr;

};
