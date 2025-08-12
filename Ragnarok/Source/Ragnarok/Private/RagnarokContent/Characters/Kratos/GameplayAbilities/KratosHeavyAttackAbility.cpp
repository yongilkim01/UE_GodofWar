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

void UKratosHeavyAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	SetKratosAttackingState(true);

	UseComboCount = CurComboCount;

	if (true == URagnarokAbilityFunctionLibrary::HasActorGameplayTag(GetKratosFromActorInfo(), JumpTag))
	{
		CurComboCount = HeavyAttackMontageMap.Num();
	}

	// 점프 어택인 경우 MovementMode를 Flying으로 변경하여 Z축 루트모션 보장
	if (CurComboCount == HeavyAttackMontageMap.Num())
	{
		bIsJumpAttack = true;
		if (AKratos* KratosCharacter = GetKratosFromActorInfo())
		{
			KratosCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}
	}
	else
	{
		bIsJumpAttack = false;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		HeavyAttackMontageMap[CurComboCount],
		1.0f,
		NAME_None,
		true, // 루트 모션 활성화
		1.0f,
		0.0f
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

	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("OnResetAttackComboCount"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.3f, false);
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

	FGameplayCueParameters CueParams;
	GetASCFromActorInfo()->ExecuteGameplayCue(
		FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Sounds.MeleeHit.Axe")),
		CueParams
	);

	if (true == ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, RagnarokGameplayTags::Global_Event_HitReact, Payload);
	}

	Debug::Print(TEXT("Hitting ") + Payload.Target.GetName() + TEXT(" with heavy attack (Combo: ") + FString::FromInt(UseComboCount) + TEXT(")"), FColor::Cyan);

}

void UKratosHeavyAttackAbility::OnMontageCompleted()
{
	Super::OnMontageCompleted();
	
	// 점프 어택이었다면 MovementMode를 Walking으로 복원
	if (bIsJumpAttack)
	{
		if (AKratos* KratosCharacter = GetKratosFromActorInfo())
		{
			KratosCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		bIsJumpAttack = false;
	}
}

void UKratosHeavyAttackAbility::OnMontageBlendOut()
{
	Super::OnMontageBlendOut();
	
	// 점프 어택이었다면 MovementMode를 Walking으로 복원
	if (bIsJumpAttack)
	{
		if (AKratos* KratosCharacter = GetKratosFromActorInfo())
		{
			KratosCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		bIsJumpAttack = false;
	}
}

void UKratosHeavyAttackAbility::OnMontageInterrupted()
{
	Super::OnMontageInterrupted();
	
	// 점프 어택이었다면 MovementMode를 Walking으로 복원
	if (bIsJumpAttack)
	{
		if (AKratos* KratosCharacter = GetKratosFromActorInfo())
		{
			KratosCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		bIsJumpAttack = false;
	}
}

void UKratosHeavyAttackAbility::OnMontageCancelled()
{
	Super::OnMontageCancelled();
	
	// 점프 어택이었다면 MovementMode를 Walking으로 복원
	if (bIsJumpAttack)
	{
		if (AKratos* KratosCharacter = GetKratosFromActorInfo())
		{
			KratosCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		bIsJumpAttack = false;
	}
}
