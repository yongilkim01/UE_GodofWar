// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "PrimaryAssetIdDataTable.generated.h"

// 일반 재료 데이터 열
USTRUCT(BlueprintType)
struct FPrimaryAssetIdDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FPrimaryAssetIdDataTableRow() {}
	~FPrimaryAssetIdDataTableRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPrimaryAssetType PrimaryAssetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AssetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AssetName;
};

/**
 * 
 */
UCLASS()
class RAGNAROK_API UPrimaryAssetIdDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
