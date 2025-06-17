// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
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
	
	template<typename  T>
	void LoadAssetData(EPrimaryAssetType AssetType)
	{
		FString PrimaryAssetType = GetPrimaryAssetType(AssetType)->ToString();
		FString PrimaryAssetName = GetPrimaryAssetName(AssetType)->ToString();

		FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(
			*PrimaryAssetType,
			*PrimaryAssetName);

		LoadPrimaryAsset
		(
			PrimaryAssetId,
			{},
			FStreamableDelegate::CreateLambda([this, PrimaryAssetId]()
				{
					UObject* AssetObject = UAssetManager::Get().GetPrimaryAssetObject(PrimaryAssetId);

					T* PrimaryAssetData = Cast<T>(AssetObject);

					if (nullptr != PrimaryAssetData)
					{
						PrimaryAssetData->LoadAsset();
					}

				})
		);
	}

public:	
	TMap<EPrimaryAssetType, TPair<FName, FName>> PrimaryIdMap;
};
