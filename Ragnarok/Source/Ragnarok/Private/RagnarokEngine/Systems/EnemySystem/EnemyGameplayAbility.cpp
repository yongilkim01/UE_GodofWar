// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/EnemySystem/EnemyGameplayAbility.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyCharacter.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyCombatComponent.h"

void UEnemyGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

AEnemyCharacter* UEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
    if (false == EnemyCharacter.IsValid())
    {
        EnemyCharacter = Cast<AEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }

    return EnemyCharacter.IsValid() ? EnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}
