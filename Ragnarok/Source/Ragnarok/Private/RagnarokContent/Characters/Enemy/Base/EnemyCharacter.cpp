// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/Base/EnemyCharacter.h"
#include "RagnarokContent/Characters/Enemy/Components/EnemyCombatComponent.h"
#include "RagnarokContent/Characters/Enemy/Components/EnemyUIComponent.h"
#include "RagnarokContent/Characters/Enemy/DataAssets/EnemyStartUpDataAsset.h"
#include "RagnarokEngine/Asset/RagnarokAssetManager.h"
#include "RagnarokContent/Abilities/DataAssets/StartUpDataAsset.h"
#include "RagnarokContent/UI/Widgets/RagnarokUserWidget.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokContent/Items/Weapons/RagnarokWeapon.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "RagnarokContent/UI/Widgets/RagnarokUserWidget.h"

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
	EnemyWidgetComponent = CreateDefaultSubobject< UWidgetComponent>(TEXT("EnemyWidgetComponent"));
	EnemyWidgetComponent->SetupAttachment(GetMesh());

}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	URagnarokUserWidget* EnemyHealthWidget = Cast<URagnarokUserWidget>(EnemyWidgetComponent->GetUserWidgetObject());

	if (nullptr != EnemyHealthWidget)
	{
		EnemyHealthWidget->InitEnemyCreatedWidet(this);
	}
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

	if (USkeletalMeshComponent* EnemyMesh = GetMesh())
	{
		EnemyMesh->bPauseAnims = true;
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	FStreamableManager& StreamableManager = URagnarokAssetManager::GetStreamableManager();
	TWeakObjectPtr<AEnemyCharacter> WeakThis(this);
	TWeakObjectPtr<USkeletalMeshComponent> WeakMesh(GetMesh());

	StreamableManager.RequestAsyncLoad(
		DeathNiagaraEffect.ToSoftObjectPath(),
		[WeakThis, WeakMesh, DeathNiagaraEffect]()
		{
			if (!WeakThis.IsValid() || !WeakMesh.IsValid())
			{
				return;
			}

			AEnemyCharacter* Enemy = WeakThis.Get();
			USkeletalMeshComponent* MeshComp = WeakMesh.Get();
			UNiagaraSystem* NiagaraSystem = DeathNiagaraEffect.Get();

			if (!NiagaraSystem || !MeshComp)
			{
				return;
			}

			UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				NiagaraSystem,
				MeshComp,
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true,
				true
			);

			if (NiagaraComponent)
			{
				UMaterialInstanceDynamic* DynMat = MeshComp->CreateDynamicMaterialInstance(0, MeshComp->GetMaterial(0));
				if (DynMat)
				{
					FLinearColor Color;
					DynMat->GetVectorParameterValue(TEXT("DissolveEdgeColor"), Color);
					NiagaraComponent->SetNiagaraVariableLinearColor(TEXT("DissolveParticleColor"), Color);
				}
			}

			if (UWorld* World = Enemy->GetWorld())
			{
				Enemy->DissolveElapsed = 0.0f;

				World->GetTimerManager().SetTimer(
					Enemy->DissolveTimerHandle,
					[WeakThis]()
					{
						if (!WeakThis.IsValid())
						{
							return;
						}

						AEnemyCharacter* Self = WeakThis.Get();
						if (UWorld* InnerWorld = Self->GetWorld())
						{
							Self->DissolveElapsed += InnerWorld->GetDeltaSeconds();
							float Alpha = FMath::Clamp(Self->DissolveElapsed / Self->DissolveDuration, 0.0f, 1.0f);

							if (USkeletalMeshComponent* Mesh = Self->GetMesh())
							{
								Mesh->SetScalarParameterValueOnMaterials(TEXT("DissolveAmount"), Alpha);
							}

							if (ARagnarokWeapon* Weapon = Self->GetEnemyCombatComponent()->GetCurrentEquippedWeapon())
							{
								if (USkeletalMeshComponent* WeaponMesh = Weapon->GetWeaponMesh())
								{
									WeaponMesh->SetScalarParameterValueOnMaterials(TEXT("DissolveAmount"), Alpha);
								}
							}

							if (Alpha >= 1.0f)
							{
								InnerWorld->GetTimerManager().ClearTimer(Self->DissolveTimerHandle);

								if (ARagnarokWeapon* Weapon = Self->GetEnemyCombatComponent()->GetCurrentEquippedWeapon())
								{
									Weapon->Destroy();
								}

								Self->Destroy();
							}
						}
					},
					0.05f,
					true
				);
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
