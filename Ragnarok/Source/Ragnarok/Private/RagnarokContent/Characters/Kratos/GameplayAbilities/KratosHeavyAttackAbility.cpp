// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosHeavyAttackAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilityFunctionLibrary.h"

void UKratosHeavyAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	if (true == URagnarokAbilityFunctionLibrary::HasActorGameplayTag(GetKratosFromActorInfo(), JumpTag))
	{
		CurComboCount = HeavyAttackMontageMap.Num();
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		HeavyAttackMontageMap[CurComboCount],
		1.0f,
		NAME_None,
		false,
		0.0f,
		false
	);

	if (nullptr != GetKratosFromActorInfo())
	{
		if (GetKratosFromActorInfo()->GetKratosAttackCount() == 0)
		{
			GetKratosFromActorInfo()->GetCharacterMovement()->DisableMovement();
		}

		GetKratosFromActorInfo()->AddKratosAttackCount(1);
	}

	if (CurComboCount == HeavyAttackMontageMap.Num())
	{
		OnResetAttackComboCount();
	}
	else
	{
		CurComboCount++;
	}

	if (nullptr != MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UKratosHeavyAttackAbility::OnMontageCompleted);
		MontageTask->OnBlendOut.AddDynamic(this, &UKratosHeavyAttackAbility::OnMontageBlendOut);
		MontageTask->OnInterrupted.AddDynamic(this, &UKratosHeavyAttackAbility::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UKratosHeavyAttackAbility::OnMontageCancelled);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}


}

void UKratosHeavyAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (nullptr != GetKratosFromActorInfo())
	{
		GetKratosFromActorInfo()->AddKratosAttackCount(-1);

		if (GetKratosFromActorInfo()->GetKratosAttackCount() <= 0)
		{
			GetKratosFromActorInfo()->SetKratosAttackCount(0);
			GetKratosFromActorInfo()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}

	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("OnResetAttackComboCount"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.3f, false);
}

void UKratosHeavyAttackAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosHeavyAttackAbility::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosHeavyAttackAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosHeavyAttackAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosHeavyAttackAbility::OnResetAttackComboCount()
{
	CurComboCount = 1;
	URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), JumpTag);

}
