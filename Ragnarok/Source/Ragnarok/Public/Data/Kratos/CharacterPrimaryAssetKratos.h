// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/RagnarokPrimaryDataAsset.h"
#include "CharacterPrimaryAssetKratos.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UCharacterPrimaryAssetKratos : public URagnarokPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void LoadAsset() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> KratosSkeletalMesh;	
};
