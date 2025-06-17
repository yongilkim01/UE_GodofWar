// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemPrimaryAssetKratosWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UItemPrimaryAssetKratosWeapon : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
};
