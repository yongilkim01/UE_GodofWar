// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosHeavyAttackAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

void UKratosHeavyAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Debug::Print(TEXT("Heavy Attack"));

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);


}

void UKratosHeavyAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UKratosHeavyAttackAbility::OnMontageCompleted()
{
}

void UKratosHeavyAttackAbility::OnMontageBlendOut()
{
}

void UKratosHeavyAttackAbility::OnMontageInterrupted()
{
}

void UKratosHeavyAttackAbility::OnMontageCancelled()
{
}

void UKratosHeavyAttackAbility::OnResetAttackComboCount()
{
}
