// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Weapons/LeviathanAxe.h"

#include "RagnarokEngine/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokContent/Characters/Kratos/Weapons/DataAssets/ItemPrimaryAssetKratosWeapon.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

void ALeviathanAxe::InitWeapon()
{
	Super::InitWeapon();

	USkeletalMesh* LoadedWeaponMesh = KratosWeaponPDA->WeaponMesh.Get();

	if (nullptr != LoadedWeaponMesh)
	{
		GetWeaponMesh()->SetSkeletalMesh(LoadedWeaponMesh);
	}
	bInitialized = true;
	OnWeaponInitialized.Broadcast();
}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();

	FOnPrimaryAssetLoadedDelegate PrimaryAssetDataDelegate;

	PrimaryAssetDataDelegate.BindUObject(this, &ALeviathanAxe::LoadWeaponPrimaryDataAsset);

	URagnarokAssetManager::Get().LoadPrimaryAssetData(
		EPrimaryAssetType::EPT_Item_LeviathanAxe,
		PrimaryAssetDataDelegate);
}

void ALeviathanAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeviathanAxe::LoadWeaponDataAsset()
{
	Super::LoadWeaponDataAsset();
}

void ALeviathanAxe::LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject)
{
	Super::LoadWeaponPrimaryDataAsset(PDAAssetObject);

	UItemPrimaryAssetKratosWeapon* LoadedPDA = Cast<UItemPrimaryAssetKratosWeapon>(PDAAssetObject);

	if (nullptr == LoadedPDA)
	{
		Debug::Print(TEXT("Kratos Weapon PDA is nullptr!!"), FColor::Red);
		return;
	}

	KratosWeaponPDA = LoadedPDA;

	if (nullptr == KratosWeaponPDA->WeaponMesh)
	{
		FSoftObjectPath MeshPath = KratosWeaponPDA->WeaponMesh.ToSoftObjectPath();

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			MeshPath,
			FStreamableDelegate::CreateLambda([this]()
				{
					if (USkeletalMesh* LoadedWeaponMesh = KratosWeaponPDA->WeaponMesh.Get())
					{
						GetWeaponMesh()->SetSkeletalMesh(LoadedWeaponMesh);
					}
				})
		);
	}
	else
	{
		if (USkeletalMesh* LoadedWeaponMesh = KratosWeaponPDA->WeaponMesh.Get())
		{
			GetWeaponMesh()->SetSkeletalMesh(LoadedWeaponMesh);
		}
	}
}
