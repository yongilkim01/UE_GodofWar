// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosRollGameplayAbility.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameplayTagContainer.h"


void UKratosRollGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	FGameplayTagContainer Container;
	GetASCFromActorInfo()->GetBlockedAbilityTags(Container);
	FGameplayTagContainer AssetTagContainer = GetAssetTags();


	if (nullptr != RollingAnimMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			RollingAnimMontage,
			1.0f,
			NAME_None,
			false,
			1.0f, 
			0.0f
		);

		if (nullptr != MontageTask)
		{
			MontageTask->OnCompleted.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageCompleted);
			MontageTask->OnBlendOut.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageBlendOut);
			MontageTask->OnInterrupted.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageInterrupted);
			MontageTask->OnCancelled.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageCancelled);
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

void UKratosRollGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
