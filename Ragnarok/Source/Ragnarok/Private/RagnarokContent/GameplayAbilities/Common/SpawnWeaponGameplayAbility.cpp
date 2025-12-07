// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/GameplayAbilities/Common/SpawnWeaponGameplayAbility.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokContent/Items/Weapons/RagnarokWeapon.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"
#include "RagnarokEngine/CombatSystem/CombatComponent.h"

USpawnWeaponGameplayAbility::USpawnWeaponGameplayAbility()
{
	AbilityActivationEvent = EAbilityActivationEvent::EAE_Given;
}

void USpawnWeaponGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (nullptr == ActorInfo || false == ActorInfo->AvatarActor.IsValid())
	{
		BreakAbility(
			TEXT("Failed ActivateAbility method in USpawnWeaponGameplayAbility"), 
			Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();
	FRotator SpawnRotation = ActorInfo->AvatarActor->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = ActorInfo->AvatarActor.Get();
	SpawnParams.Instigator = Cast<APawn>(ActorInfo->AvatarActor.Get());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UWorld* WorldContext = ActorInfo->AvatarActor->GetWorld();
	
	if (nullptr == WorldContext)
	{
		BreakAbility(
			TEXT("Failed ActivateAbility method in USpawnWeaponGameplayAbility"), 
			Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	RagnarokWeapon = WorldContext->SpawnActor<ARagnarokWeapon>(
		WeaponClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams);

	if (false == IsValid(RagnarokWeapon))
	{
		BreakAbility(
			TEXT("Failed ActivateAbility method in USpawnWeaponGameplayAbility"),
			Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	CharacterActorInfo = ActorInfo;

	if (false == RagnarokWeapon->IsInitialize())
	{
		AttachWeaponToCharacter();
		if (ARagnarokCharacter* RagnarokCharacter = Cast<ARagnarokCharacter>(CharacterActorInfo->AvatarActor.Get()))
		{
			RagnarokWeapon->SetOwner(RagnarokCharacter);
		}
	}
	else
	{
		RagnarokWeapon->OnWeaponInitialized.AddDynamic(this, &USpawnWeaponGameplayAbility::AttachWeaponToCharacter);

	}


	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void USpawnWeaponGameplayAbility::AttachWeaponToCharacter()
{
	USkeletalMeshComponent* ParentMesh = nullptr;
	if (true == CharacterActorInfo->AvatarActor.IsValid())
	{
		APawn* Pawn = Cast<APawn>(CharacterActorInfo->AvatarActor.Get());
		if (nullptr != Pawn)
		{
			ParentMesh = Pawn->FindComponentByClass<USkeletalMeshComponent>();
		}
	}

	if (nullptr != ParentMesh)
	{
		FAttachmentTransformRules AttachRules(
			EAttachmentRule::SnapToTarget,    // LocationRule
			EAttachmentRule::KeepRelative,    // RotationRule
			EAttachmentRule::KeepWorld,       // ScaleRule
			true						      // bWeldSimulatedBodies
		);

		RagnarokWeapon->AttachToComponent(
			ParentMesh,
			AttachRules,
			SocketNameToAttachTo // FName 타입의 소켓 이름으로 무기 부착
		);
	}

	GetCombatComponentFromActorInfo()->RegisterSpawnWeapon(SpawnWeaponTag, RagnarokWeapon, bEquip);
}
