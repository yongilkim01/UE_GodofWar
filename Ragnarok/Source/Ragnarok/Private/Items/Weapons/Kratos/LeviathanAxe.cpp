// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Kratos/LeviathanAxe.h"

#include "Manager/Global/RagnarokAssetManager.h"
#include "Data/Items/Weapons/ItemPrimaryAssetKratosWeapon.h"
#include "Core/Tools/RagnarokDebugHelper.h"

void ALeviathanAxe::LoadWeaponDataAsset()
{
	Super::LoadWeaponDataAsset();
}

void ALeviathanAxe::LoadWeaponPrimaryDataAsset()
{
	Super::LoadWeaponPrimaryDataAsset();

	FString PrimaryAssetType = URagnarokAssetManager::Get().GetPrimaryAssetType(EPrimaryAssetType::EPT_Item_LeviathanAxe)->ToString();
	FString PrimaryAssetName = URagnarokAssetManager::Get().GetPrimaryAssetName(EPrimaryAssetType::EPT_Item_LeviathanAxe)->ToString();

	WeaponPDAId = FPrimaryAssetId(
		*PrimaryAssetType,
		*PrimaryAssetName);

	URagnarokAssetManager::Get().LoadPrimaryAsset
	(
		WeaponPDAId,
		{},
		FStreamableDelegate::CreateUObject(this, &ALeviathanAxe::AsyncLoadItemLeviathanAxe)
	);
}

void ALeviathanAxe::AsyncLoadItemLeviathanAxe()
{
	UObject* AssetObject = UAssetManager::Get().GetPrimaryAssetObject(WeaponPDAId);
	WeaponPDA = Cast<UItemPrimaryAssetKratosWeapon>(AssetObject);

	if (nullptr != WeaponPDA && true == WeaponPDA->WeaponMesh.IsValid())
	{
		GetWeaponMesh()->SetSkeletalMesh(WeaponPDA->WeaponMesh.Get());
	}
	else if (nullptr != WeaponPDA)
	{
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad
		(
			WeaponPDA->WeaponMesh.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([this]()
				{
					if (USkeletalMesh* Mesh = WeaponPDA->WeaponMesh.Get())
					{
						GetWeaponMesh()->SetSkeletalMesh(Mesh);
					}
				}));
	}
	else
	{
		Debug::Print(TEXT("Kratos config primary data assets is nullptr!!"), FColor::Red);

	}
}
