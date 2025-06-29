// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RagnarokContentTypes.generated.h"

class UKratosLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FKratosWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UKratosLinkedAnimLayer> WeaponAnimLayer;
};