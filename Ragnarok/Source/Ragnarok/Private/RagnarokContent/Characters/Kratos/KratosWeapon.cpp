// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"

#include "GameFramework/ProjectileMovementComponent.h"

AKratosWeapon::AKratosWeapon()
{
	PivotPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PivotPoint"));
	PivotPointComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("WeaponProjectileMovement"));
	ProjectileMovementComponent->SetAutoActivate(false);
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
}

void AKratosWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AKratosWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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

void AKratosWeapon::LoadWeaponDataAsset()
{
	Super::LoadWeaponDataAsset();
}

void AKratosWeapon::LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject)
{
	Super::LoadWeaponPrimaryDataAsset(PDAAssetObject);
}