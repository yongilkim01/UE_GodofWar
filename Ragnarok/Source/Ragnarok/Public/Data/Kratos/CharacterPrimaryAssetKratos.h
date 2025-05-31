// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterPrimaryAssetKratos.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UCharacterPrimaryAssetKratos : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> KratosSkeletalMesh;	
};
