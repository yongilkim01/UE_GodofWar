// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosEquipWeaponGameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UKratosEquipWeaponGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (nullptr == EquipWeaponMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		EquipWeaponMontage,
		1.0f,
		NAME_None,
		false,
		0.0f,
		false
	);

	if (nullptr != Task)
	{
		Task->OnCompleted.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnMontageCompleted);
		Task->OnBlendOut.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnMontageBlendOut);
		Task->OnInterrupted.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnMontageInterrupted);
		Task->OnCancelled.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnMontageCancelled);
		Task->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UKratosEquipWeaponGameplayAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosEquipWeaponGameplayAbility::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosEquipWeaponGameplayAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosEquipWeaponGameplayAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}
