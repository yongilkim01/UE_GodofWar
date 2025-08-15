// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosDashHeavyAttackAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"

#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/CombatSystem/Tags/CombatGameplayTags.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UKratosDashHeavyAttackAbility::UKratosDashHeavyAttackAbility()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosDashHeavyAttackAbility::UKratosDashHeavyAttackAbility"));

}

void UKratosDashHeavyAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosDashHeavyAttackAbility::ActivateAbility"));

	SetKratosAttackingState(true);
	ExecuteAttackMontage();

	HitWaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CombatGameplayTags::Combat_Event_MeleeHit,
		nullptr,
		false,
		false
	);

	if (nullptr != HitWaitEventTask)
	{
		HitWaitEventTask->EventReceived.AddDynamic(this, &UKratosDashHeavyAttackAbility::OnHitEventReceived);
		HitWaitEventTask->ReadyForActivation();
	}
}

void UKratosDashHeavyAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosDashHeavyAttackAbility::EndAbility"));

	if (nullptr != HitWaitEventTask)
	{
		HitWaitEventTask->EndTask();
		HitWaitEventTask = nullptr;
	}

	SetKratosAttackingState(false);
}

bool UKratosDashHeavyAttackAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
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
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		Debug::Print(TEXT("Kratos is nullptr"), FColor::Red);
		return false;
	}
}

void UKratosDashHeavyAttackAbility::OnHitEventReceived(FGameplayEventData Payload)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosDashHeavyAttackAbility::OnHitEventReceived"));

	int32 UseComboCount = 1;

	float WeaponDamage = GetKratosCombatComponent()->GetKratosEquippedWeaponDamageAtLevel(GetAbilityLevel());

	FGameplayEffectSpecHandle SpecHandle = CreateKratosDamageEffectSpecHandle(
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

void UKratosDashHeavyAttackAbility::ExecuteAttackMontage()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosDashHeavyAttackAbility::ExecuteAttackMontage"));

	if (nullptr == AttackMontageToPlay)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontageToPlay,
		AnimMontageRate,
		NAME_None,
		true,
		1.0f,
		0.0f
	);

	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->OnCompleted.AddDynamic(this, &UKratosDashHeavyAttackAbility::OnMontageCompleted);
		AttackMontageTask->OnBlendOut.AddDynamic(this, &UKratosDashHeavyAttackAbility::OnMontageBlendOut);
		AttackMontageTask->OnInterrupted.AddDynamic(this, &UKratosDashHeavyAttackAbility::OnMontageInterrupted);
		AttackMontageTask->OnCancelled.AddDynamic(this, &UKratosDashHeavyAttackAbility::OnMontageCancelled);
		AttackMontageTask->ReadyForActivation();
	}
	else
	{
		Debug::Print(TEXT("UKratosHeavyAttackAbility::ExecuteAttackMontage - Failed creating montage task"), FColor::Orange);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosDashHeavyAttackAbility::OnMontageCompleted()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosDashHeavyAttackAbility::OnMontageCompleted"));

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosDashHeavyAttackAbility::OnMontageBlendOut()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosDashHeavyAttackAbility::OnMontageBlendOut"));
}

void UKratosDashHeavyAttackAbility::OnMontageInterrupted()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosDashHeavyAttackAbility::OnMontageInterrupted"));
}

void UKratosDashHeavyAttackAbility::OnMontageCancelled()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosDashHeavyAttackAbility::OnMontageCancelled"));
}
