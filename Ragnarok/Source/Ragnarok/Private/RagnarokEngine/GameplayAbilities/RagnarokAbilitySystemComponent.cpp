// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"

void URagnarokAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (false == InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (false  == AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag))
		{
			continue;
		}

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void URagnarokAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}
