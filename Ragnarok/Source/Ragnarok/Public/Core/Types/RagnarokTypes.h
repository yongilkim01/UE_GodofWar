// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RagnarokTypes.generated.h"

UENUM(BlueprintType)
enum class EPrimaryAssetType : uint8
{
	EPT_None				UMETA(DisplayName = "None"),
	EPT_CHARACTER_KRATOS    UMETA(DisplayName = "Chracter Kratos"),
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
