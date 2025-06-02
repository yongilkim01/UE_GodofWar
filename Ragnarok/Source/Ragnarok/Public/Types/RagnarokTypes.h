// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RagnarokTypes.generated.h"

UENUM(BlueprintType)
enum class EPrimaryAssetType : uint8
{
	EPT_None	UMETA(DisplayName = "None"),
	EPT_CHARACTER_KRATOS    UMETA(DisplayName = "Chracter Kratos"),
};


/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokTypes : public UObject
{
	GENERATED_BODY()
	
};
