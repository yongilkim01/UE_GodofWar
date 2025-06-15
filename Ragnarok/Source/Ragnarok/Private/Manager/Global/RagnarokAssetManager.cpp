// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Global/RagnarokAssetManager.h"
#include "Core/Tools/RagnarokDebugHelper.h"
#include "Data/Global/PrimaryAssetIdDataTable.h"
#include "Data/Items/Weapons/ItemPrimaryAssetKratosWeapon.h"
#include "Data/Kratos/CharacterPrimaryAssetKratos.h"

URagnarokAssetManager::URagnarokAssetManager()
{
}

void URagnarokAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Tables/Global/DT_PrimaryAssetId.DT_PrimaryAssetId"));

	if (DataTable)
	{
		for (const auto& Row : DataTable->GetRowMap())
		{
			FPrimaryAssetIdDataTableRow* PrimaryAssetIdDataRow = (FPrimaryAssetIdDataTableRow*)Row.Value;

			PrimaryIdMap.Add(PrimaryAssetIdDataRow->PrimaryAssetType, TPair<FName, FName>());

			PrimaryIdMap[PrimaryAssetIdDataRow->PrimaryAssetType].Key = PrimaryAssetIdDataRow->AssetType;
			PrimaryIdMap[PrimaryAssetIdDataRow->PrimaryAssetType].Value = PrimaryAssetIdDataRow->AssetName;
		}
	}

}

URagnarokAssetManager& URagnarokAssetManager::Get()	
{
	URagnarokAssetManager* This = Cast<URagnarokAssetManager>(GEngine->AssetManager);

	if (nullptr == This)
	{
		Debug::Print(TEXT("Invalid AssetManager in DefaultEngine.ini, Must set this to RagnarokAssetManager"));
		return *NewObject<URagnarokAssetManager>();
	}
	else
	{
		return *This;
	}
}


const TPair<FName, FName>* URagnarokAssetManager::GetPrimaryAssetIdPair(EPrimaryAssetType AssetType) const
{
	return PrimaryIdMap.Find(AssetType);
}

const FName* URagnarokAssetManager::GetPrimaryAssetType(EPrimaryAssetType AssetType) const
{
	return &GetPrimaryAssetIdPair(AssetType)->Key;
}

const FName* URagnarokAssetManager::GetPrimaryAssetName(EPrimaryAssetType AssetType) const
{
	return &GetPrimaryAssetIdPair(AssetType)->Value;
}

UObject* URagnarokAssetManager::GetPDA(EPrimaryAssetType AssetType)
{
	FString PrimaryAssetType = GetPrimaryAssetType(AssetType)->ToString();
	FString PrimaryAssetName = GetPrimaryAssetName(AssetType)->ToString();

	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(
		*PrimaryAssetType,
		*PrimaryAssetName);

	return UAssetManager::Get().GetPrimaryAssetObject(PrimaryAssetId);
}

void URagnarokAssetManager::LoadPrimaryAssetData(EPrimaryAssetType AssetType, FOnPrimaryAssetLoadedDelegate OnLoadedCallback)
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
		FStreamableDelegate::CreateLambda([this, PrimaryAssetId, OnLoadedCallback]()
		{
			UObject* AssetObject = UAssetManager::Get().GetPrimaryAssetObject(PrimaryAssetId);

			if (true == OnLoadedCallback.IsBound())
			{
				OnLoadedCallback.Execute(AssetObject);
			}
		})
	);
}
