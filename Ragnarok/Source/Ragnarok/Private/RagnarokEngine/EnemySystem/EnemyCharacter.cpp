// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/EnemySystem/EnemyCharacter.h"
#include "RagnarokEngine/EnemySystem/EnemyCombatComponent.h"
#include "RagnarokEngine/EnemySystem/EnemyStartUpDataAsset.h"
#include "RagnarokEngine/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokEngine/DataAssets/StartUpData/StartUpDataAsset.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "GameFramework/CharacterMovementComponent.h"

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

					Debug::Print(TEXT("Enemy Start up data loaded"), FColor::Green);
				}
			}
		)
	);

}
