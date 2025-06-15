// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Kratos/KratosWeapon.h"
#include "LeviathanAxe.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API ALeviathanAxe : public AKratosWeapon
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

private:


};
