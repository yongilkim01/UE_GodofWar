// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/EnemySystem/EnemyCharacter.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyCombatComponent.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyStartUpDataAsset.h"
#include "RagnarokEngine/Systems/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokEngine/Systems/AbilitySystem/DataAssets/StartUpDataAsset.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Objects/Items/Weapons/RagnarokWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

AEnemyCharacter::AEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f;

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));

}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

UCombatComponent* AEnemyCharacter::GetCombatComponent() const
{
	return EnemyCombatComponent;
}

void AEnemyCharacter::Die()
{
	Super::Die();

	if (nullptr != GetMesh())
	{
		GetMesh()->bPauseAnims = true;
	}

	if (nullptr != GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (nullptr != GetMesh())
	{
		DissolveElapsed = 0.0f;
		GetWorld()->GetTimerManager().SetTimer(DissolveTimerHandle, [this]()
			{
				DissolveElapsed += GetWorld()->GetDeltaSeconds();
				float Alpha = FMath::Clamp(DissolveElapsed / DissolveDuration, 0.0f, 1.0f);
				GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveAmount"), Alpha);
				if (nullptr != GetEnemyCombatComponent()->GetCurrentEquippedWeapon())
				{
					GetEnemyCombatComponent()->GetCurrentEquippedWeapon()->GetWeaponMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveAmount"), Alpha);
				}
				if (Alpha >= 1.0f)
				{
					GetWorld()->GetTimerManager().ClearTimer(DissolveTimerHandle);
					if (nullptr != GetEnemyCombatComponent()->GetCurrentEquippedWeapon())
					{
						GetEnemyCombatComponent()->GetCurrentEquippedWeapon()->Destroy();
					}
					this->Destroy();

				}
			}, 0.01f, true);
	}

}

void AEnemyCharacter::InitEnemyStartUpData()
{
	if (true == StartUpData.IsNull())
	{
		return;
	}

	URagnarokAssetManager::GetStreamableManager().RequestAsyncLoad(
		StartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (UStartUpDataAsset* LoadedData = StartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
				}
			}
		)
	);

}
