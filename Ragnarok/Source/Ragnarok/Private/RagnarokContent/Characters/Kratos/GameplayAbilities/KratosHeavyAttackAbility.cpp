// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosHeavyAttackAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilityFunctionLibrary.h"
#include "RagnarokEngine/Systems/CombatSystem/Tags/CombatGameplayTags.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

UKratosHeavyAttackAbility::UKratosHeavyAttackAbility()
{
	CurComboCount = 1;
	bShowDebug = false;
}

void UKratosHeavyAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::ActivateAbility"));

	SetKratosAttackingState(true);
	bReserveComboAttack = false;
	ExecuteAttackMontage(CurComboCount);

	HitWaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CombatGameplayTags::Combat_Event_MeleeHit,
		nullptr,
		false,
		false
	);

	if (nullptr != HitWaitEventTask)
	{
		HitWaitEventTask->EventReceived.AddDynamic(this, &UKratosHeavyAttackAbility::OnGameplayEventReceived);
		HitWaitEventTask->ReadyForActivation();
	}

	AttackWaitStartTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		KratosGameplayTags::Kratos_Event_AttackWait_Start,
		nullptr,
		false,
		false
	);

	if (nullptr != AttackWaitStartTask)
	{
		AttackWaitStartTask->EventReceived.AddDynamic(this, &UKratosHeavyAttackAbility::OnAttackWaitStartEventRecived);
		AttackWaitStartTask->ReadyForActivation();
	}

	AttackWaitEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		KratosGameplayTags::Kratos_Event_AttackWait_End,
		nullptr,
		false,
		false
	);

	if (nullptr != AttackWaitEndTask)
	{
		AttackWaitEndTask->EventReceived.AddDynamic(this, &UKratosHeavyAttackAbility::OnAttackWaitEndEventRecived);
		AttackWaitEndTask->ReadyForActivation();

	}
}

void UKratosHeavyAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::EndAbility"));

	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->EndTask();
		AttackMontageTask = nullptr;
	}

	if (nullptr != HitWaitEventTask)
	{
		HitWaitEventTask->EndTask();
		HitWaitEventTask = nullptr;
	}

	if (nullptr != AttackWaitStartTask)
	{
		AttackWaitStartTask->EndTask();
		AttackWaitStartTask = nullptr;
	}

	if (nullptr != AttackWaitEndTask)
	{
		AttackWaitEndTask->EndTask();
		AttackWaitEndTask = nullptr;
	}

	bReserveComboAttack = false;
	SetKratosAttackingState(false);
	Kratos->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	CurComboCount = 1;
	URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), JumpTag);

}

void UKratosHeavyAttackAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::InputPressed"));

	switch (CurAttackState)
	{
	case ERagnarokAttackState::ERAS_Attacking:
		bReserveComboAttack = true;
		break;
	case ERagnarokAttackState::ERAS_AttackWait:
		ProcessNextCombo();
		break;
	default:
		break;
	}
}

bool UKratosHeavyAttackAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (false == Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AKratos* KratosCharacter = Cast<AKratos>(ActorInfo->AvatarActor.Get());
	
	if (nullptr != KratosCharacter)
	{
		if (true == KratosCharacter->IsRunning())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		Debug::Print(TEXT("Kratos is nullptr"), FColor::Red);
		return false;
	}
}

void UKratosHeavyAttackAbility::ResetAttackComboCount()
{
	CurComboCount = 1;
	URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), JumpTag);

}

void UKratosHeavyAttackAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	float WeaponDamage = GetKratosCombatComponent()->GetKratosEquippedWeaponDamageAtLevel(GetAbilityLevel());

	FGameplayEffectSpecHandle SpecHandle = CreateKratosComboDamageEffectSpecHandle(
		EffectClass,
		WeaponDamage,
		KratosGameplayTags::Kratos_SetByCaller_AttackType_Heavy,
		UseComboCount);

	AActor* TargetActor = const_cast<AActor*>(Payload.Target.Get());

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);

	FGameplayCueParameters CueParams;
	GetASCFromActorInfo()->ExecuteGameplayCue(
		FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Sounds.MeleeHit.Axe")),
		CueParams
	);

	if (true == ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, RagnarokGameplayTags::Global_Event_HitReact, Payload);
	}

	if (true == bShowDebug)  Debug::Print(TEXT("Hitting ") + Payload.Target.GetName() + TEXT(" with heavy attack (Combo: ") + FString::FromInt(UseComboCount) + TEXT(")"), FColor::Cyan);

}

void UKratosHeavyAttackAbility::OnAttackWaitStartEventRecived(FGameplayEventData Payload)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::OnAttackWaitStartEventRecived"));

	CurAttackState = ERagnarokAttackState::ERAS_AttackWait;

	if (true == bReserveComboAttack)
	{
		bReserveComboAttack = false;
		ProcessNextCombo();
	}
}

void UKratosHeavyAttackAbility::OnAttackWaitEndEventRecived(FGameplayEventData Payload)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::OnAttackWaitEndEventRecived"));

	if (ERagnarokAttackState::ERAS_AttackWait == CurAttackState)
	{
		CurAttackState = ERagnarokAttackState::ERAS_Attacking;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

	bReserveComboAttack = false;
}

void UKratosHeavyAttackAbility::ExecuteAttackMontage(int32 ComboCount)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::ExecuteAttackMontage"));

	if (false == HeavyAttackMontageMap.Contains(ComboCount))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	CurAttackState = ERagnarokAttackState::ERAS_Attacking;

	AttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		HeavyAttackMontageMap[ComboCount],
		1.0f,
		NAME_None,
		true,
		1.0f,
		0.0f
	);


	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->OnCompleted.AddDynamic(this, &UKratosHeavyAttackAbility::OnMontageCompleted);
		AttackMontageTask->OnBlendOut.AddDynamic(this, &UKratosHeavyAttackAbility::OnMontageBlendOut);
		AttackMontageTask->OnInterrupted.AddDynamic(this, &UKratosHeavyAttackAbility::OnMontageInterrupted);
		AttackMontageTask->OnCancelled.AddDynamic(this, &UKratosHeavyAttackAbility::OnMontageCancelled);
		AttackMontageTask->ReadyForActivation();
	}
	else
	{
		Debug::Print(TEXT("UKratosHeavyAttackAbility::ExecuteAttackMontage - Failed creating montage task"), FColor::Orange);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

}

void UKratosHeavyAttackAbility::ProcessNextCombo()
{
	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->EndTask();
		AttackMontageTask = nullptr;
	}

	CurComboCount++;

	if (CurComboCount > HeavyAttackMontageMap.Num())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else
	{
		ExecuteAttackMontage(CurComboCount);
	}
}

void UKratosHeavyAttackAbility::OnMontageCompleted()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::OnMontageCompleted"));

	if (ERagnarokAttackState::ERAS_AttackWait != CurAttackState)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosHeavyAttackAbility::OnMontageBlendOut()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::OnMontageBlendOut"));
}

void UKratosHeavyAttackAbility::OnMontageInterrupted()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::OnMontageInterrupted"));
}

void UKratosHeavyAttackAbility::OnMontageCancelled()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosHeavyAttackAbility::OnMontageCancelled"));
}
