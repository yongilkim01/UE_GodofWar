// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Core/Types/RagnarokTypes.h"
#include "RagnarokAssetManager.generated.h"

DECLARE_DELEGATE_OneParam(FOnPrimaryAssetLoadedDelegate, UObject* /* LoadedPDA */)

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	URagnarokAssetManager();
	virtual void StartInitialLoading() override;
	static URagnarokAssetManager& Get();


public:
	const TPair<FName, FName>* GetPrimaryAssetIdPair(EPrimaryAssetType AssetType) const;
	const FName* GetPrimaryAssetType(EPrimaryAssetType AssetType) const;
	const FName* GetPrimaryAssetName(EPrimaryAssetType AssetType) const;
	UObject* GetPDA(EPrimaryAssetType AssetType);

	void LoadPrimaryAssetData(EPrimaryAssetType AssetType, FOnPrimaryAssetLoadedDelegate OnLoadedCallback);
	

public:	
	TMap<EPrimaryAssetType, TPair<FName, FName>> PrimaryIdMap;
};
