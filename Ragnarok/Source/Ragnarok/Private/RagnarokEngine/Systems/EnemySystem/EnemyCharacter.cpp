// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/EnemySystem/EnemyCharacter.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyCombatComponent.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyUIComponent.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyStartUpDataAsset.h"
#include "RagnarokEngine/Systems/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokEngine/Systems/AbilitySystem/DataAssets/StartUpDataAsset.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Objects/Items/Weapons/RagnarokWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

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
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));

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

void AEnemyCharacter::Die(TSoftObjectPtr<UNiagaraSystem> DeathNiagaraEffect)
{
	Super::Die(DeathNiagaraEffect);

	if (nullptr != GetMesh())
	{
		GetMesh()->bPauseAnims = true;
	}

	if (nullptr != GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	FStreamableManager& StreamableManager = URagnarokAssetManager::GetStreamableManager();
	USkeletalMeshComponent* MeshComp = GetMesh();

	StreamableManager.RequestAsyncLoad(
		DeathNiagaraEffect.ToSoftObjectPath(),
		[DeathNiagaraEffect, MeshComp, this]()
		{
			UNiagaraSystem* DeathNiagaraSystem = DeathNiagaraEffect.Get();
			if (nullptr != DeathNiagaraSystem && nullptr != MeshComp)
			{
				UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
					DeathNiagaraSystem,
					MeshComp,
					NAME_None,
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					EAttachLocation::KeepRelativeOffset,
					true,
					true
				);
				UMaterialInstanceDynamic* InstanceDynamic = MeshComp->CreateDynamicMaterialInstance(0, MeshComp->GetMaterial(0));
				FLinearColor LinearColor;
				InstanceDynamic->GetVectorParameterValue(TEXT("DissolveEdgeColor"), LinearColor);
				NiagaraComponent->SetNiagaraVariableLinearColor(TEXT("DissolveParticleColor"), LinearColor);

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
					}, 0.05f, true);
			}
		}
	);
}

URagnarokUIComponent* AEnemyCharacter::GetUIComponent() const
{
	return EnemyUIComponent;
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
