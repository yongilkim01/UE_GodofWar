// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KratosConfigPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class RAGNAROK_API UKratosConfigPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> KratosSkeletalMesh;
	
};
	