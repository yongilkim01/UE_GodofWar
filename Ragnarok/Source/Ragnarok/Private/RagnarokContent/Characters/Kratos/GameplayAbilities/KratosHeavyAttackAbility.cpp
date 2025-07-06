// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosHeavyAttackAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilityFunctionLibrary.h"
#include "RagnarokEngine/Systems/CombatSystem/Tags/CombatGameplayTags.h"

void UKratosHeavyAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	UseComboCount = CurComboCount;

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

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CombatGameplayTags::Combat_Event_MeleeHit,
		nullptr,
		false,
		false
	);

	if (nullptr != WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UKratosHeavyAttackAbility::OnGameplayEventReceived);

		if (true == WaitEventTask->EventReceived.IsBound())
		{
			Debug::Print(TEXT("HeavyAttack Delegate Binding Sucess"));
		}
		else
		{
			Debug::Print(TEXT("HeavyAttack Delegate Binding Failed"));
		}

		WaitEventTask->ReadyForActivation();
	}

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

void UKratosHeavyAttackAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	float WeaponDamage = GetKratosCombatComponent()->GetKratosEquippedWeaponDamageAtLevel(GetAbilityLevel());

	FGameplayEffectSpecHandle SpecHandle = CreateKratosDamageEffectSpecHandle(
		EffectClass,
		WeaponDamage,
		KratosGameplayTags::Kratos_SetByCaller_AttackType_Heavy,
		UseComboCount);

	AActor* TargetActor = const_cast<AActor*>(Payload.Target.Get());

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);

	if (true == ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, RagnarokGameplayTags::Global_Event_HitReact, Payload);
	}

	Debug::Print(TEXT("Hitting ") + Payload.Target.GetName() + TEXT(" with heavy attack (Combo: ") + FString::FromInt(UseComboCount) + TEXT(")"), FColor::Cyan);

}
		