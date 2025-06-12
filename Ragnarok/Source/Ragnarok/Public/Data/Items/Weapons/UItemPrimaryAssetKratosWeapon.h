// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UItemPrimaryAssetKratosWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UUItemPrimaryAssetKratosWeapon : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
};
