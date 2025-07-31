// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/Abilities/EnemyGameplayAbility.h"
#include "RagnarokContent/Characters/Enemy/Base/EnemyCharacter.h"
#include "RagnarokContent/Characters/Enemy/Components/EnemyCombatComponent.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"

void UEnemyGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEnemyGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

FGameplayEffectSpecHandle UEnemyGameplayAbility::CreateEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
    check(EffectClass);
    
    FGameplayEffectContextHandle ContextHandle = GetASCFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetASCFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        ContextHandle
    );

    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        RagnarokGameplayTags::Global_SetByCaller_BaseDamage,
        InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
    );

    return EffectSpecHandle;

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
