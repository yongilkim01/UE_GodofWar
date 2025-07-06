// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosHitPauseGameplayAbility.h"
#include "Kismet/GameplayStatics.h"

void UKratosHitPauseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (nullptr != TriggerEventData)
    {
        FGameplayTag EventTag = TriggerEventData->EventTag;
        float EventMagnitude = TriggerEventData->EventMagnitude;

        if (nullptr != ActorInfo && true == ActorInfo->AvatarActor.IsValid())
        {
            UObject* WorldContextObject = ActorInfo->AvatarActor.Get();
            UGameplayStatics::SetGlobalTimeDilation(WorldContextObject, 0.1f);

            FTimerHandle TimerHandle;
            UWorld* World = WorldContextObject->GetWorld();

            if (nullptr != World)
            {
                World->GetTimerManager().SetTimer(
                    TimerHandle,
                    [this, Handle, ActorInfo, ActivationInfo, WorldContextObject]()
                    {
                        UGameplayStatics::SetGlobalTimeDilation(WorldContextObject, 1.0f);
                        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
                    },
                    0.01f,
                    false
                );
            }
        }
    }
}
void UKratosHitPauseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UKratosHitPauseGameplayAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosHitPauseGameplayAbility::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosHitPauseGameplayAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosHitPauseGameplayAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}
