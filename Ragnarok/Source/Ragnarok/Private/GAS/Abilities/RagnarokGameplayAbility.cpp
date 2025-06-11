// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/RagnarokGameplayAbility.h"

#include "GAS/RagnarokAbilitySystemComponent.h"

void URagnarokGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (EAbilityActivationEvent::EAE_Given == AbilityActivationEvent)
	{
		if (nullptr != ActorInfo && true == Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void URagnarokGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (EAbilityActivationEvent::EAE_Given == AbilityActivationEvent)
	{
		if (nullptr != ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}
