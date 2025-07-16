// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/EnemySystem/GameplayAbilities/EnemyMeleeAttackGameplayAbility.h"
#include "RagnarokEngine/Systems/CombatSystem/Tags/CombatGameplayTags.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokContent/Characters/Enemy/EnemyGameplayTags.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UEnemyMeleeAttackGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    CurrentSpecHandle = Handle;
    CurrentActorInfo = ActorInfo;
    CurrentActivationInfo = ActivationInfo;

    UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this,
        NAME_None,
        AttackMontage,
        1.0f,
        NAME_None,
        false,
        0.0f,
        false
    );

    WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
        this,
        CombatGameplayTags::Combat_Event_MeleeHit,
        nullptr,
        false,
        false
    );

    if (nullptr != WaitEventTask)
    {
        Debug::Print(TEXT("WaitEventTask Created"));

        WaitEventTask->EventReceived.AddDynamic(this, &UEnemyMeleeAttackGameplayAbility::OnGameplayEventReceived);

        if (WaitEventTask->EventReceived.IsBound())
        {
            Debug::Print(TEXT("Equip Delegate Binding Success"));
        }
        else
        {
            Debug::Print(TEXT("Equip Delegate Binding Failed"));
        }

        WaitEventTask->ReadyForActivation();
    }


    if (nullptr != MontageTask)
    {
        MontageTask->OnCompleted.AddDynamic(this, &UEnemyMeleeAttackGameplayAbility::OnMontageCompleted);
        MontageTask->OnBlendOut.AddDynamic(this, &UEnemyMeleeAttackGameplayAbility::OnMontageBlendOut);
        MontageTask->OnInterrupted.AddDynamic(this, &UEnemyMeleeAttackGameplayAbility::OnMontageInterrupted);
        MontageTask->OnCancelled.AddDynamic(this, &UEnemyMeleeAttackGameplayAbility::OnMontageCancelled);
        MontageTask->ReadyForActivation();
    }
    else
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }
}

void UEnemyMeleeAttackGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UEnemyMeleeAttackGameplayAbility::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UEnemyMeleeAttackGameplayAbility::OnMontageBlendOut()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UEnemyMeleeAttackGameplayAbility::OnMontageInterrupted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UEnemyMeleeAttackGameplayAbility::OnMontageCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UEnemyMeleeAttackGameplayAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
    FGameplayEffectSpecHandle SpecHandle = CreateEnemyDamageEffectSpecHandle(
        EffectClass,
        DamageScalableFloat
    );

    AActor* TargetActor = const_cast<AActor*>(Payload.Target.Get());

    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);

    if (true == ActiveGameplayEffectHandle.WasSuccessfullyApplied())
    {
        //UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, RagnarokGameplayTags::Global_Event_HitReact, Payload);
    }

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
    