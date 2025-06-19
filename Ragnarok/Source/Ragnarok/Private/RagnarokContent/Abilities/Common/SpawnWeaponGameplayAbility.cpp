// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Abilities/Common/SpawnWeaponGameplayAbility.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/GameItem/Weapon/RagnarokWeapon.h"

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

	ARagnarokWeapon* RagnarokWeapon = WorldContext->SpawnActor<ARagnarokWeapon>(
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
	
	USkeletalMeshComponent* ParentMesh = nullptr;
	if (true == ActorInfo->AvatarActor.IsValid())
	{
		APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
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
			SocketNameToAttachTo // FName 타입의 멤버 변수여야 함
		);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
