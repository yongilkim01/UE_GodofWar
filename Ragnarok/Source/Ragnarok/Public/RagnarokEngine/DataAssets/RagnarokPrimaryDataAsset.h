// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RagnarokPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual void LoadAsset();
	
};
