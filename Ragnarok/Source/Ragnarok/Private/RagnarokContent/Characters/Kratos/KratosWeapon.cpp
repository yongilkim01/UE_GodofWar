// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/Animation/KratosLinkedAnimLayer.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"

AKratosWeapon::AKratosWeapon()
{
	PivotPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PivotPoint"));
	PivotPointComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("WeaponProjectileMovement"));
	ProjectileMovementComponent->SetAutoActivate(false);
	ProjectileMovementComponent->InitialSpeed = 3500.0f;
	ProjectileMovementComponent->MaxSpeed = 3500.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.3f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->SetUpdatedComponent(GetWeaponMesh());
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

void AKratosWeapon::UnlinkAnimClassLayersFromActor(const AActor* UnlinkActor)
{
	if (nullptr == UnlinkActor)
	{
		Debug::Print(TEXT("AKratosWeapon::UnlinkAnimClassLayersFromActor method's UnlinkActor is nullptr"), FColor::Red);
		return;
	}

	USkeletalMeshComponent* ParentMesh = UnlinkActor->FindComponentByClass<USkeletalMeshComponent>();

	if (nullptr != ParentMesh)
	{
		ParentMesh->UnlinkAnimClassLayers(WeaponData.WeaponAnimLayer.Get());

	}
}

void AKratosWeapon::UnUequipWeapon(const AActor* OwnerActor, FName HandleSocketName)
{
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,    // LocationRule
		EAttachmentRule::KeepRelative,    // RotationRule
		EAttachmentRule::KeepWorld,       // ScaleRule
		true						      // bWeldSimulatedBodies
	);

	AttachWeaponToActor(OwnerActor, AttachmentRules, HandleSocketName);
	UnlinkAnimClassLayersFromActor(OwnerActor);
}