// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/EnemySystem/GameplayAbilities/EnemyDeathGameplayAbility.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyCharacter.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UEnemyDeathGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (nullptr != TriggerEventData)
	{
		FGameplayTag EventTag = TriggerEventData->EventTag;
		float EventMagnitude = TriggerEventData->EventMagnitude;

        if (true == bHasDeathMontageToPlay)
        {
            int32 RandomIndex = FMath::RandRange(0, AnimMontageArray.Num() - 1);

            UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
                this,
                NAME_None,
                AnimMontageArray[RandomIndex],
                1.0f,
                NAME_None,
                false,
                0.0f,
                false
            );

            FGameplayCueParameters CueParams;
            GetASCFromActorInfo()->ExecuteGameplayCue(
                FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Sounds.Death.Gruntling")),
                CueParams
            );

            if (nullptr != MontageTask)
            {
                MontageTask->OnCompleted.AddDynamic(this, &UEnemyDeathGameplayAbility::OnMontageCompleted);
                MontageTask->OnBlendOut.AddDynamic(this, &UEnemyDeathGameplayAbility::OnMontageBlendOut);
                MontageTask->OnInterrupted.AddDynamic(this, &UEnemyDeathGameplayAbility::OnMontageInterrupted);
                MontageTask->OnCancelled.AddDynamic(this, &UEnemyDeathGameplayAbility::OnMontageCancelled);
                MontageTask->ReadyForActivation();
            }
            else
            {
                EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
            }
        }
        else
        {
            EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        }
	}

}

void UEnemyDeathGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UEnemyDeathGameplayAbility::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyDeathGameplayAbility::OnMontageBlendOut()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyDeathGameplayAbility::OnMontageInterrupted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyDeathGameplayAbility::OnMontageCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
