// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "ScalableFloat.h"
#include "RagnarokContentTypes.generated.h"

class UKratosLinkedAnimLayer;
class UInputMappingContext;

UENUM(BlueprintType)
enum class ERagnarokWeaponState : uint8
{
	ERWS_None			UMETA(DisplayName = "None"),
	ERWS_Unequipped		UMETA(DisplayName = "Equipped"),
	ERWS_Equipped		UMETA(DisplayName = "Equipped"),
	ERWS_Throw			UMETA(DisplayName = "Throw"),
	ERWS_Lodge			UMETA(DisplayName = "Lodge"),
	ERWS_Recall			UMETA(DisplayName = "Recall"),
};

UENUM(BLueprintType)
enum class EArcMovementType : uint8
{
	EAMT_Jump			UMETA(DisplayName = "Jump"),
	EAMT_Leap			UMETA(DisplayName = "Leap"),
	EAMT_Parabolic		UMETA(DisplayName = "Parabolic"),
	EAMT_Custom			UMETA(DisplayName = "Custom")
};

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
	ERD_BackwardLeft	UMETA(DisplayName = "BackwardLeft"),
	ERD_Custom			UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class ERagnarokAttackState : uint8
{
	ERAS_None			UMETA(DisplayName = "None"),
	ERAS_Attacking		UMETA(DisplayName = "Attacking"),
	ERAS_AttackWait		UMETA(DisplayName = "AttackWait"),
	ERAS_Recovery		UMETA(DisplayName = "Recovery")
};

UENUM(BlueprintType)
enum class ERagnarokRollState : uint8
{
	ERRS_None			UMETA(DisplayName = "None"),
	ERRS_Dodge			UMETA(DisplayName = "Dodge"),
	ERRS_Roll			UMETA(DisplayName = "Roll")
};

UENUM(BlueprintType)
enum class ERagnarokAttackReactType : uint8
{
	ERART_None			UMETA(DisplayName = "None"),
	ERART_Knockback		UMETA(DisplayName = "Knockback"),
	ERART_Launch		UMETA(DisplayName = "Launch"),
	ERART_Slamdown		UMETA(DisplayName = "Slamdown")
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

USTRUCT(BlueprintType)
struct FKratosAttackData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int32 ComboIndex = 0;

	UPROPERTY(EditAnywhere)
	FName ComboSectionName;

	UPROPERTY(EditAnywhere)
	ERagnarokAttackReactType AttackReactType = ERagnarokAttackReactType::ERART_None;
};