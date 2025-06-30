// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"

void AKratosWeapon::InitWeapon()
{
	Super::InitWeapon();
}

void AKratosWeapon::AssignGratnAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandleArray)
{
	GrantedAbilitySpecHandleArray = InSpecHandleArray;
}

TArray<FGameplayAbilitySpecHandle>& AKratosWeapon::GetGrantedAbilitySpecHandleArray()
{
	return GrantedAbilitySpecHandleArray;
}

void AKratosWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AKratosWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKratosWeapon::LoadWeaponDataAsset()
{
	Super::LoadWeaponDataAsset();
}

void AKratosWeapon::LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject)
{
	Super::LoadWeaponPrimaryDataAsset(PDAAssetObject);
}
