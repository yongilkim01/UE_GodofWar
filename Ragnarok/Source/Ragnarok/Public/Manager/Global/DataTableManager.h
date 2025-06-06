// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/Types/RagnarokTypes.h"
#include "DataTableManager.generated.h"

struct FPrimaryAssetIdDataTableRow;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UDataTableManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static const FPrimaryAssetIdDataTableRow* GetPrimaryAssetId(UWorld* World, EPrimaryAssetType AssetType);
};
