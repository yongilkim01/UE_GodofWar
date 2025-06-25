// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/DataAssets/KratosStartUpDataAsset.h"

#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokGameplayAbility.h"

bool FKratosAbilitySet::IsVaild() const
{
	return InputTag.IsValid() && Ability;
}

void UKratosStartUpDataAsset::GiveToAbilitySystemComponent(URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASC, ApplyLevel);

	for (const FKratosAbilitySet& KratosAbilitySet : KratosStartUpAbilitySetArray)
	{
		if (false == KratosAbilitySet.IsVaild())
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(KratosAbilitySet.Ability);
		AbilitySpec.SourceObject = InASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(KratosAbilitySet.InputTag);

		InASC->GiveAbility(AbilitySpec);
	}


}
