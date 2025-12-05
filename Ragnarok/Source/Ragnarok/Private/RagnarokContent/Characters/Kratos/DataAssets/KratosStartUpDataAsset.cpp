// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/DataAssets/KratosStartUpDataAsset.h"

#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokGameplayAbility.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"

void UKratosStartUpDataAsset::GiveToAbilitySystemComponent(URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASC, ApplyLevel);

	for (const FRagnarokbilitySet& KratosAbilitySet : KratosStartUpAbilitySetArray)
	{
		if (false == KratosAbilitySet.IsValid())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(KratosAbilitySet.Ability);

		AbilitySpec.SourceObject = InASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(KratosAbilitySet.InputTag);

		FGameplayAbilitySpecHandle SpecHandle = InASC->GiveAbility(AbilitySpec);
	}
}
