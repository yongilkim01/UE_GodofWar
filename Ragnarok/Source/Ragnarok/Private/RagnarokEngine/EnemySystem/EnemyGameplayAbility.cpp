// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/EnemySystem/EnemyGameplayAbility.h"
#include "RagnarokEngine/EnemySystem/EnemyCharacter.h"
#include "RagnarokEngine/EnemySystem/EnemyCombatComponent.h"

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
