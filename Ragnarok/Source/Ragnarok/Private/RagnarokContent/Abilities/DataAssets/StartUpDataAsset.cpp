// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Abilities/DataAssets/StartUpDataAsset.h"

#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokGameplayAbility.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokGameplayEffect.h"

void UStartUpDataAsset::GiveToAbilitySystemComponent(URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel)
{
	check(InASC);

	GrantAbilities(ActivateOnGivenAbilityArray, InASC, ApplyLevel);
	GrantAbilities(ReactiveAbilityArray, InASC, ApplyLevel);

	if (false == StartUpGameplayEffectArray.IsEmpty())
	{
		for (const TSubclassOf<URagnarokGameplayEffect>& EffectClass : StartUpGameplayEffectArray)
		{
			if (!EffectClass)
			{
				continue;
			}

			URagnarokGameplayEffect* GameplayEffect = EffectClass->GetDefaultObject<URagnarokGameplayEffect>();

			InASC->ApplyGameplayEffectToSelf(
				GameplayEffect,
				ApplyLevel,
				InASC->MakeEffectContext()
			);
		}
	}
}

void UStartUpDataAsset::GrantAbilities(const TArray<TSubclassOf<URagnarokGameplayAbility>>& InGrantAbilityArray, URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel)
{
	if (true == InGrantAbilityArray.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<URagnarokGameplayAbility>& Ability : InGrantAbilityArray)
	{
		if (nullptr == Ability)
		{
			return;
		}

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASC->GiveAbility(AbilitySpec);
	}
}
