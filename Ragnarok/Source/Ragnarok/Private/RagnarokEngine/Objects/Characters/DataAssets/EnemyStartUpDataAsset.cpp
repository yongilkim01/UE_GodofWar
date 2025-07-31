// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Objects/Characters/DataAssets/EnemyStartUpDataAsset.h"
#include "RagnarokEngine/Systems/AbilitySystem/Abilities/EnemyGameplayAbility.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

void UEnemyStartUpDataAsset::GiveToAbilitySystemComponent(URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASC, ApplyLevel);

	if (false == EnemyCombatAbilityArray.IsEmpty())
	{
		for (const TSubclassOf<UEnemyGameplayAbility> AbilityClass : EnemyCombatAbilityArray)
		{
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InASC->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			InASC->GiveAbility(AbilitySpec);
		}
	}
}
