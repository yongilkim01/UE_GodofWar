// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RagnarokTypes.generated.h"

DECLARE_DELEGATE_OneParam(FOnMeshLoadedDelegate, USkeletalMesh*);

UENUM(BlueprintType)
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

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokTypes : public UObject
{
	GENERATED_BODY()
	
};
