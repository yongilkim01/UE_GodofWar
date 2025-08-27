// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosRecallWeaponAbility.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UKratosRecallWeaponAbility::UKratosRecallWeaponAbility()
{
	bShowDebug = true;
}

void UKratosRecallWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosRecallWeaponAbility::ActivateAbility"));

	RecallWeapon();

	PlayRecallAnimMontage();

	//EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UKratosRecallWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosRecallWeaponAbility::EndAbility"));

}

bool UKratosRecallWeaponAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (false == Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (true == IsPossibleRecallWeapon())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UKratosRecallWeaponAbility::PlayRecallAnimMontage()
{
	if (nullptr == RecallAnimMontage)
	{
		Debug::Print(TEXT("UKratosRecallWeaponAbility::RecallAnimMontage is nullptr"), FColor::Red);
		return;
	}

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		RecallAnimMontage,
		1.0f,
		NAME_None,
		true,
		1.0f,
		0.037f,
		false
	);

	if (nullptr != PlayMontageTask)
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &UKratosRecallWeaponAbility::OnMontageCompleted);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &UKratosRecallWeaponAbility::OnMontageBlendOut);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UKratosRecallWeaponAbility::OnMontageInterrupted);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UKratosRecallWeaponAbility::OnMontageCancelled);
		PlayMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosRecallWeaponAbility::RecallWeapon()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosRecallWeaponAbility::RecallWeapon"));

}

bool UKratosRecallWeaponAbility::IsPossibleRecallWeapon() const
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosRecallWeaponAbility::IsPossibleRecallWeapon"));

	return true;
}
