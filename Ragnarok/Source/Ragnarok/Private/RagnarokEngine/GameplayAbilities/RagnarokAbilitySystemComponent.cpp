// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokGameplayAbility.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"

void URagnarokAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (false == InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (false  == AbilitySpec.GetDynamicSpecSourceTags().HasTag(InInputTag))
		{
			continue;
		}

		UGameplayAbility* ActiveAbility = AbilitySpec.GetPrimaryInstance();

		if (nullptr != ActiveAbility && true == ActiveAbility->IsActive())
		{
			ActiveAbility->InputPressed(AbilitySpec.Handle, AbilityActorInfo.Get(), FGameplayAbilityActivationInfo());
		}
		else
		{
			TryActivateAbility(AbilitySpec.Handle);
		}

	}
}

void URagnarokAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (false == InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (false == AbilitySpec.GetDynamicSpecSourceTags().HasTag(InInputTag))
		{
			continue;
		}

		UGameplayAbility* ActiveAbility = AbilitySpec.GetPrimaryInstance();

		if (nullptr != ActiveAbility && true == ActiveAbility->IsActive())
		{
			ActiveAbility->InputReleased(AbilitySpec.Handle, AbilityActorInfo.Get(), FGameplayAbilityActivationInfo());
		}
	}
}

void URagnarokAbilitySystemComponent::GrantWeaponAbilities(const TArray<FRagnarokbilitySet> InWeaponAbilityArray, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandleArray)
{
	if (true == InWeaponAbilityArray.IsEmpty())
	{
		return;
	}

	for (const FRagnarokbilitySet& AbilitySet : InWeaponAbilityArray)
	{
		if (false == AbilitySet.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilitySet.Ability);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);


		OutGrantedAbilitySpecHandleArray.AddUnique(GiveAbility(AbilitySpec));
	}
}

void URagnarokAbilitySystemComponent::RemoveWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandleArray)
{
	if (true == InSpecHandleArray.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandleArray)
	{
		if (true == SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	InSpecHandleArray.Empty();
}

bool URagnarokAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag ActivateTag)
{
	check(ActivateTag.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecArray;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(ActivateTag.GetSingleTagContainer(), FoundAbilitySpecArray);

	if (false == FoundAbilitySpecArray.IsEmpty())
	{
		const int32 RandomIndex = FMath::RandRange(0, FoundAbilitySpecArray.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecArray[RandomIndex];

		check(SpecToActivate);

		if (false == SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}

	}

	return false;
}
