// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/RagnarokWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Manager/Global/RagnarokAssetManager.h"

ARagnarokWeapon::ARagnarokWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(GetRootComponent());
	WeaponCollision->SetBoxExtent(FVector(20.0f));
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARagnarokWeapon::BeginPlay()
{
	Super::BeginPlay();

	LoadWeaponDataAsset();
	//LoadWeaponPrimaryDataAsset();
}

void ARagnarokWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARagnarokWeapon::LoadWeaponDataAsset()
{
}

void ARagnarokWeapon::LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject)
{

}
