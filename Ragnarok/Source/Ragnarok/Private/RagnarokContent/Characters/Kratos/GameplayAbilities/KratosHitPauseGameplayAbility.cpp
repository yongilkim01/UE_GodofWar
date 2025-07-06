// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosHitPauseGameplayAbility.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/Camera/KratosMeleeCameraShake.h"

#include "Kismet/GameplayStatics.h"

void UKratosHitPauseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (nullptr != TriggerEventData)
    {
        FGameplayTag EventTag = TriggerEventData->EventTag;
        float EventMagnitude = TriggerEventData->EventMagnitude;

        CurrentActorInfo = ActorInfo;

        if (nullptr != ActorInfo && ActorInfo->AvatarActor.IsValid())
        {
            AKratosController* CurKratosController = GetKratosControllerFromActorInfo();
            UObject* WorldContextObject = ActorInfo->AvatarActor.Get();
            TSubclassOf<UCameraShakeBase> LocalCameraShakeClass = CameraShakeClass;
            FGameplayAbilitySpecHandle LocalHandle = Handle;
            FGameplayAbilityActivationInfo LocalActivationInfo = ActivationInfo;
            const FGameplayAbilityActorInfo* LocalActorInfo = ActorInfo;

            UGameplayStatics::SetGlobalTimeDilation(WorldContextObject, 0.1f);

            FTimerHandle TimerHandle;
            UWorld* World = WorldContextObject->GetWorld();

            if (nullptr != World && CurKratosController)
            {
                World->GetTimerManager().SetTimer(
                    TimerHandle,
                    [CurKratosController, WorldContextObject, LocalCameraShakeClass, LocalHandle, LocalActorInfo, LocalActivationInfo, this]()
                    {
                        UGameplayStatics::SetGlobalTimeDilation(WorldContextObject, 1.0f);
                        CurKratosController->ClientStartCameraShake(LocalCameraShakeClass);
                        this->EndAbility(LocalHandle, LocalActorInfo, LocalActivationInfo, true, false);
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
