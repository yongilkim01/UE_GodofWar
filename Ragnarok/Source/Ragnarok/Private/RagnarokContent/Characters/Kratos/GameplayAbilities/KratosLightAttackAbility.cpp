// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosLightAttackAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"


#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilityFunctionLibrary.h"
#include "RagnarokEngine/Systems/CombatSystem/Tags/CombatGameplayTags.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

/*
	크레토스가 첫번째 공격을 했다 
	-> CurComboCount, UseComboCount는 둘다 1이다 
	-> CurComboCount를 1을 더해서 2가 된다.
	-> 애니메이션 실행 도중 현재 공격 상태가 AttackWait상태가 됀다.
	-> Attack Wait 도중 추가적인 공격을 할 경우와 아닌 경우로 나뉘게 된다.

		-> 다음 공격이 들어왔을 경우에는 현재
*/

UKratosLightAttackAbility::UKratosLightAttackAbility()
{
	AttackWaitPositionMap.Add(1, 0.7f);
	AttackWaitPositionMap.Add(2, 0.8f);
	AttackWaitPositionMap.Add(3, 0.75f);
}

void UKratosLightAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Handle, ActorInfo, ActivationInfo 할당
	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	// Timer 초기화
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	// 사용하는 콤보 카운트와 현재 콤보 카운트 분리, 현재 공격상태 공격 모드로 설정
	UseComboCount = CurComboCount;
	CurAttackState = ERagnarokAttackState::ERAS_Attacking;

	// 몽타주 실행
	AttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		LightAttackMontageMap[CurComboCount],
		1.0f,
		NAME_None,
		false,
		0.0f,
		false
	);

	// 
	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CombatGameplayTags::Combat_Event_MeleeHit,
		nullptr,
		false,
		false
	);

	if (nullptr != WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UKratosLightAttackAbility::OnGameplayEventReceived);

		if (true == WaitEventTask->EventReceived.IsBound())
		{
			Debug::Print(TEXT("LightAttack Delegate Binding Sucess"));
		}
		else
		{
			Debug::Print(TEXT("LightAttack Delegate Binding Failed"));
		}

		WaitEventTask->ReadyForActivation();
	}

	
	//if (nullptr != GetKratosFromActorInfo())
	//{
	//	if (0 == GetKratosFromActorInfo()->GetKratosAttackCount())
	//	{
	//		GetKratosFromActorInfo()->GetCharacterMovement()->DisableMovement();
	//	}

	//	GetKratosFromActorInfo()->AddKratosAttackCount(1);
	//}

	// 현재 코보 카운트가 약공격의 개수하고 같다면 Reset 메소드 호출
	//if (CurComboCount == LightAttackMontageMap.Num())
	//{
	//	OnResetAttackComboCount();
	//}
	//else
	//{
	//	if (CurComboCount + 1 == LightAttackMontageMap.Num())
	//	{
	//		URagnarokAbilityFunctionLibrary::AddGameplayTagToActor(
	//			GetKratosFromActorInfo(), JumpTag);
	//	}

	//	CurComboCount++;
	//}

	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->OnCompleted.AddDynamic(this, &UKratosLightAttackAbility::OnAttackMontageCompleted);
		AttackMontageTask->OnBlendOut.AddDynamic(this, &UKratosLightAttackAbility::OnMontageBlendOut);
		AttackMontageTask->OnInterrupted.AddDynamic(this, &UKratosLightAttackAbility::OnMontageInterrupted);
		AttackMontageTask->OnCancelled.AddDynamic(this, &UKratosLightAttackAbility::OnMontageCancelled);
		AttackMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}


}

void UKratosLightAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ERagnarokAttackState::ERAS_AttackWait == CurAttackState)
	{
		ResumeMontage();
		ExitAttackWaitState();
	}

	CurAttackState = ERagnarokAttackState::ERAS_None;

	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->EndTask();
		AttackMontageTask = nullptr;
	}


	//if (nullptr != GetKratosFromActorInfo())
	//{
	//	GetKratosFromActorInfo()->AddKratosAttackCount(-1);

	//	if (GetKratosFromActorInfo()->GetKratosAttackCount() <= 0)
	//	{
	//		GetKratosFromActorInfo()->SetKratosAttackCount(0);
	//		GetKratosFromActorInfo()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	//	}
	//}

	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("OnResetAttackComboCount"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.3f, false);
}

void UKratosLightAttackAbility::OnResetAttackComboCount()
{
	CurComboCount = 1;
	URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), JumpTag);
	
}

void UKratosLightAttackAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	float WeaponDamage = GetKratosCombatComponent()->GetKratosEquippedWeaponDamageAtLevel(GetAbilityLevel());

	FGameplayEffectSpecHandle SpecHandle = CreateKratosDamageEffectSpecHandle(
		EffectClass,
		WeaponDamage,
		KratosGameplayTags::Kratos_SetByCaller_AttackType_Light,
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

	Debug::Print(TEXT("Hitting ") + Payload.Target.GetName() + TEXT(" with light attack (Combo: ") + FString::FromInt(UseComboCount) + TEXT(")"), FColor::Cyan);
}

void UKratosLightAttackAbility::OnAttackMontageCompleted()
{
	if (ERagnarokAttackState::ERAS_AttackWait != CurAttackState)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}


void UKratosLightAttackAbility::StartAttackWaitState()
{
	if (ERagnarokAttackState::ERAS_Attacking != CurAttackState)
	{
		return;
	}

	CurAttackState = ERagnarokAttackState::ERAS_AttackWait;

	UAnimInstance* AnimInstance = GetKratosFromActorInfo()->GetMesh()->GetAnimInstance();

	if (nullptr != AnimInstance && nullptr != LightAttackMontageMap[UseComboCount])
	{
		AnimInstance->Montage_SetPlayRate(LightAttackMontageMap[UseComboCount], 0.0f);
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("OnAttackWaitTimeOut"));
	GetWorld()->GetTimerManager().SetTimer(AttackWaitTimerHandle, TimerDelegate, ComboWaitDuration, false);
}

void UKratosLightAttackAbility::OnAttackWaitTimeOut()
{
	// 현재 공격 상태가 기다리고 있는 상태가 아니라면
	if (ERagnarokAttackState::ERAS_AttackWait == CurAttackState)
	{
		ExitAttackWaitState();
		ResumeMontage();
	}
}

void UKratosLightAttackAbility::OnNextComboInput()
{
	if (ERagnarokAttackState::ERAS_AttackWait == CurAttackState)
	{
		ExitAttackWaitState();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosLightAttackAbility::ExitAttackWaitState()
{
	CurAttackState = ERagnarokAttackState::ERAS_Recovery;
	GetWorld()->GetTimerManager().ClearTimer(AttackWaitTimerHandle);
}

void UKratosLightAttackAbility::PauseMontageAtPosition(float Position)
{
}

void UKratosLightAttackAbility::ResumeMontage()
{
	UAnimInstance* AnimInstance = GetKratosFromActorInfo()->GetMesh()->GetAnimInstance();
	if (nullptr != AnimInstance && nullptr != LightAttackMontageMap[UseComboCount])
	{
		AnimInstance->Montage_SetPlayRate(LightAttackMontageMap[UseComboCount], 1.0f);
	}
}
