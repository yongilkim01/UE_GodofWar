// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/EnemySystem/GameplayAbilities/EnemyHitReactGameplayAbility.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyCharacter.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include "Kismet/KismetMathLibrary.h"

void UEnemyHitReactGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (nullptr != TriggerEventData)
    {
        FGameplayTag EventTag = TriggerEventData->EventTag;
        float EventMagnitude = TriggerEventData->EventMagnitude;

        if (true == bRotateToATargetActor)
        {
            FVector AttackerLocation = TriggerEventData->Instigator.Get()->GetActorLocation();
            FVector EnemyLocation = GetEnemyCharacterFromActorInfo()->GetActorLocation();
            FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, AttackerLocation);

            GetEnemyCharacterFromActorInfo()->SetActorRotation(LookAtRotation);

            if (true == bHasHitReactMontagesToPlay)
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

                if (nullptr != MontageTask)
                {
                    MontageTask->OnCompleted.AddDynamic(this, &UEnemyHitReactGameplayAbility::OnMontageCompleted);
                    MontageTask->OnBlendOut.AddDynamic(this, &UEnemyHitReactGameplayAbility::OnMontageBlendOut);
                    MontageTask->OnInterrupted.AddDynamic(this, &UEnemyHitReactGameplayAbility::OnMontageInterrupted);
                    MontageTask->OnCancelled.AddDynamic(this, &UEnemyHitReactGameplayAbility::OnMontageCancelled);
                    MontageTask->ReadyForActivation();
                }
                else
                {
                    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
                }
            }
            else
            {
                FTimerHandle TimerHandle;

                GetWorld()->GetTimerManager().SetTimer(
                    TimerHandle,
                    [this]()
                    {
                        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
                    },
                    2.0f,
                    false
                );
            }
        }
    }
}

void UEnemyHitReactGameplayAbility::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyHitReactGameplayAbility::OnMontageBlendOut()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyHitReactGameplayAbility::OnMontageInterrupted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyHitReactGameplayAbility::OnMontageCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}