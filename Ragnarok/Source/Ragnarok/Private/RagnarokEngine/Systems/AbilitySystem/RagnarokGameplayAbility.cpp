// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AbilitySystem/RagnarokGameplayAbility.h"

#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/CombatSystem/CombatComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"

void URagnarokGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URagnarokGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (EAbilityActivationEvent::EAE_Given == AbilityActivationEvent)
	{
		if (nullptr != ActorInfo && false == Spec.IsActive())
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

bool URagnarokGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bBlocked = false;
	auto CheckForBlocked = [&](const FGameplayTagContainer& ContainerA, const FGameplayTagContainer& ContainerB)
		{
			if (ContainerA.IsEmpty() || ContainerB.IsEmpty() || !ContainerA.HasAny(ContainerB))
			{
				return;
			}

			if (OptionalRelevantTags)
			{
				if (!bBlocked)
				{
					UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
					const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
					OptionalRelevantTags->AddTag(BlockedTag);
				}

				OptionalRelevantTags->AppendMatchingTags(ContainerA, ContainerB);
			}

			bBlocked = true;
		};

	bool bMissing = false;
	auto CheckForRequired = [&](const FGameplayTagContainer& TagsToCheck, const FGameplayTagContainer& RequiredTags)
		{
			if (RequiredTags.IsEmpty() || TagsToCheck.HasAll(RequiredTags))
			{
				return;
			}

			if (OptionalRelevantTags)
			{
				if (!bMissing)
				{
					UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
					const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;
					OptionalRelevantTags->AddTag(MissingTag);
				}

				FGameplayTagContainer MissingTags = RequiredTags;
				MissingTags.RemoveTags(TagsToCheck.GetGameplayTagParents());
				OptionalRelevantTags->AppendTags(MissingTags);
			}

			bMissing = true;
		};

	CheckForBlocked(GetAssetTags(), AbilitySystemComponent.GetBlockedAbilityTags());
	CheckForBlocked(AbilitySystemComponent.GetOwnedGameplayTags(), ActivationBlockedTags);

	if (SourceTags != nullptr)
	{
		CheckForBlocked(*SourceTags, SourceBlockedTags);
	}
	if (TargetTags != nullptr)
	{
		CheckForBlocked(*TargetTags, TargetBlockedTags);
	}

	CheckForRequired(AbilitySystemComponent.GetOwnedGameplayTags(), ActivationRequiredTags);
	if (SourceTags != nullptr)
	{
		CheckForRequired(*SourceTags, SourceRequiredTags);
	}
	if (TargetTags != nullptr)
	{
		CheckForRequired(*TargetTags, TargetRequiredTags);
	}

	return !bBlocked && !bMissing;
}

void URagnarokGameplayAbility::BreakAbility(const FString& ErrorMsg, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Debug::Print(ErrorMsg, FColor::Red);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	return;
}

UCombatComponent* URagnarokGameplayAbility::GetCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UCombatComponent>();
}

URagnarokAbilitySystemComponent* URagnarokGameplayAbility::GetASCFromActorInfo() const
{
	return Cast<URagnarokAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle URagnarokGameplayAbility::ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(nullptr != TargetASC && true == InSpecHandle.IsValid());

	FActiveGameplayEffectHandle EffectHandle = GetASCFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetASC
	);

	return EffectHandle;
}
