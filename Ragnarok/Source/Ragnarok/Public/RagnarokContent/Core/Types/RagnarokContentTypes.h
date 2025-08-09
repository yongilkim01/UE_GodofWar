// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "ScalableFloat.h"
#include "RagnarokContentTypes.generated.h"

class UKratosLinkedAnimLayer;
class UInputMappingContext;

UENUM(BlueprintType)
enum class ERagnarokDirection : uint8
{
	ERD_None			UMETA(DisplayName = "None"),
	ERD_Forward			UMETA(DisplayName = "Forward"),
	ERD_Backward		UMETA(DisplayName = "Backward"),
	ERD_Right			UMETA(DisplayName = "Right"),
	ERD_Left			UMETA(DisplayName = "Left"),
	ERD_ForwardRight	UMETA(DisplayName = "ForwardRight"),
	ERD_ForwardLeft		UMETA(DisplayName = "ForwardLeft"),
	ERD_BackwardRight	UMETA(DisplayName = "BackwardRight"),
	ERD_BackwardLeft	UMETA(DisplayName = "BackwardLeft")
};

UENUM(BlueprintType)
enum class ERagnarokAttackState : uint8
{
	ERAS_None		UMETA(DisplayName = "None"),
	ERAS_Attacking	UMETA(DisplayName = "Attacking"),
	ERAS_AttackWait	UMETA(DisplayName = "AttackWait"),
	ERAS_Recovery	UMETA(DisplayName = "Recovery")
};

UENUM(BlueprintType)
enum class ERagnarokRollState : uint8
{
	ERRS_None		UMETA(DisplayName = "None"),
	ERRS_Dodge		UMETA(DisplayName = "Dodge"),
	ERRS_Roll		UMETA(DisplayName = "Roll")
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