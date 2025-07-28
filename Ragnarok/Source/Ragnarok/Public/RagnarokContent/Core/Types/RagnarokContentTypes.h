// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "ScalableFloat.h"
#include "RagnarokContentTypes.generated.h"

class UKratosLinkedAnimLayer;
class UInputMappingContext;

UENUM(BlueprintType)
enum class ERagnarokAttackState : uint8
{
	ERAS_None		UMETA(DisplayName = "None"),
	ERAS_Attacking	UMETA(DisplayName = "Attacking"),
	ERAS_AttackWait	UMETA(DisplayName = "AttackWait"),
	ERAS_Recovery	UMETA(DisplayName = "Recovery")
};

USTRUCT(BlueprintType)
struct FKratosWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UKratosLinkedAnimLayer> WeaponAnimLayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FRagnarokbilitySet> WeaponAbilitySetArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FScalableFloat WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> WeaponIconTextureSoftPtr;
};