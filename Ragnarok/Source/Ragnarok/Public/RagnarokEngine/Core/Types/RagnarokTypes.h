// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "RagnarokTypes.generated.h"

class URagnarokGameplayAbility;

DECLARE_DELEGATE_OneParam(FOnMeshLoadedDelegate, USkeletalMesh*);

UENUM()
enum class ERagnarokConfirmType : uint8
{
	ERCT_Yes				UMETA(DisplayName = "Yes"),
	ERCT_No					UMETA(DisplayName = "No")
};

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	ETD_CurrentEquippedWeapon,
	ETD_LeftHand,
	ETD_RightHand
};

UENUM(BlueprintType, meta = (ScriptName = "RagnarokPrimaryAssetType"))
enum class EPrimaryAssetType : uint8
{
	EPT_None				UMETA(DisplayName = "None"),
	EPT_Chracter_Kratos		UMETA(DisplayName = "Chracter Kratos"),
	EPT_Item_LeviathanAxe	UMETA(DisplayName = "Item LeviathanAxe"),
};

UENUM(BlueprintType)
enum class EAbilityActivationEvent : uint8
{
	EAE_None				UMETA(DisplayName = "None"),
	EAE_Triggered			UMETA(DisplayName = "Triggered"),
	EAE_Given				UMETA(DisplayName = "Given"),
};

USTRUCT(BlueprintType)
struct FRagnarokbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ragnarok|InputTag")
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URagnarokGameplayAbility> Ability;

	bool IsValid() const;
};

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokTypes : public UObject
{
	GENERATED_BODY()
	
};
