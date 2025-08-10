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


#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilityFunctionLibrary.h"
#include "RagnarokEngine/Systems/CombatSystem/Tags/CombatGameplayTags.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

UKratosLightAttackAbility::UKratosLightAttackAbility()
{
	LaunchPowerMap.Add(1, 1600.0f);
	LaunchPowerMap.Add(2, 1600.0f);
	LaunchPowerMap.Add(3, 1600.0f);
	LaunchPowerMap.Add(4, 1600.0f);

	CurComboCount = 1;
}

void UKratosLightAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Debug::Print(TEXT("UKratosLightAttackAbility::ActivateAbility"));

	SetKratosAttackingState(true);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	bReserveComboAttack = false;

	ExecuteAttackMontage(CurComboCount);

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
		WaitEventTask->ReadyForActivation();
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
		AttackWaitStartTask->EventReceived.AddDynamic(this, &UKratosLightAttackAbility::OnAttackWaitStartEventRecived);
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
		AttackWaitEndTask->EventReceived.AddDynamic(this, &UKratosLightAttackAbility::OnAttackWaitEndEventRecived);
		AttackWaitEndTask->ReadyForActivation();

	}
}

void UKratosLightAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	Debug::Print(TEXT("UKratosLightAttackAbility::EndAbility"));

	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->EndTask();
		AttackMontageTask = nullptr;
	}

	if (nullptr != WaitEventTask)
	{
		WaitEventTask->EndTask();
		WaitEventTask = nullptr;
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

	ResetAttackComboCount();
}

void UKratosLightAttackAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Debug::Print(TEXT("UKratosLightAttackAbility::InputPressed"));

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

void UKratosLightAttackAbility::OnMontageCompleted()
{
	Debug::Print(TEXT("UKratosLightAttackAbility::OnMontageCompleted"));

	if (ERagnarokAttackState::ERAS_AttackWait != CurAttackState)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosLightAttackAbility::OnMontageBlendOut()
{
	Debug::Print(TEXT("UKratosLightAttackAbility::OnMontageBlendOut"));
}

void UKratosLightAttackAbility::OnMontageInterrupted()
{
	Debug::Print(TEXT("UKratosLightAttackAbility::OnMontageBlendOut"));
}

void UKratosLightAttackAbility::ResetAttackComboCount()
{
	CurComboCount = 1;
	URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), JumpTag);
	
}

void UKratosLightAttackAbility::ProcessNextCombo()
{
	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->EndTask();
		AttackMontageTask = nullptr;
	}

	CurComboCount++;

	if (LightAttackMontageMap.Num() == CurComboCount + 1)
	{
		URagnarokAbilityFunctionLibrary::AddGameplayTagToActor(
			GetKratosFromActorInfo(), JumpTag);
	}

	if (CurComboCount > LightAttackMontageMap.Num())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else
	{
		ExecuteAttackMontage(CurComboCount);
	}
}

void UKratosLightAttackAbility::LaunchCharacterForward(int32 ComboCount)
{
	if (nullptr == Kratos || false == LaunchPowerMap.Contains(ComboCount))
	{
		return;
	}

	float LaunchPower = LaunchPowerMap[ComboCount];

	FVector ForwardVector = Kratos->GetActorForwardVector();
	FVector LaunchVelocity = ForwardVector * LaunchPower;
	LaunchVelocity.Z = 0.0f;

	Kratos->LaunchCharacter(LaunchVelocity, true, false);
}

void UKratosLightAttackAbility::LaunchCharacterForwardSmoothly(int32 ComboCount)
{
	if (nullptr == Kratos || false == LaunchPowerMap.Contains(ComboCount))
	{
		return;
	}
	
	UCharacterMovementComponent* KratosMovementComponent = Kratos->GetCharacterMovement();

	float LaunchPower = LaunchPowerMap[ComboCount];

	FVector CurVelocity = KratosMovementComponent->Velocity;
	FVector ForwardVector = Kratos->GetActorForwardVector();
	FVector AddVelocity = ForwardVector * LaunchPower;

	FVector CalVelocity = CurVelocity + AddVelocity;
	CalVelocity.Z = CurVelocity.Z;

	KratosMovementComponent->Velocity = CalVelocity;
}

void UKratosLightAttackAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	float WeaponDamage = GetKratosCombatComponent()->GetKratosEquippedWeaponDamageAtLevel(GetAbilityLevel());

	FGameplayEffectSpecHandle SpecHandle = CreateKratosDamageEffectSpecHandle(
		EffectClass,
		WeaponDamage,
		KratosGameplayTags::Kratos_SetByCaller_AttackType_Light,
		CurComboCount);

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

	//Debug::Print(TEXT("Hitting ") + Payload.Target.GetName() + TEXT(" with light attack (Combo: ") + FString::FromInt(UseComboCount) + TEXT(")"), FColor::Cyan);
}

void UKratosLightAttackAbility::OnAttackWaitStartEventRecived(FGameplayEventData Payload)
{
	CurAttackState = ERagnarokAttackState::ERAS_AttackWait;

	if (true == bReserveComboAttack)
	{
		bReserveComboAttack = false;
		ProcessNextCombo();
	}
}

void UKratosLightAttackAbility::OnAttackWaitEndEventRecived(FGameplayEventData Payload)
{
	if (ERagnarokAttackState::ERAS_AttackWait == CurAttackState)
	{
		CurAttackState = ERagnarokAttackState::ERAS_Attacking;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

	bReserveComboAttack = false;
}

void UKratosLightAttackAbility::ExecuteAttackMontage(int32 ComboCount)
{
	if (false == LightAttackMontageMap.Contains(ComboCount))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	CurAttackState = ERagnarokAttackState::ERAS_Attacking;

	AttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		LightAttackMontageMap[ComboCount],
		1.0f,
		NAME_None,
		true,
		1.0f,
		0.0f,
		false
	);

	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->OnCompleted.AddDynamic(this, &UKratosLightAttackAbility::OnMontageCompleted);
		AttackMontageTask->OnBlendOut.AddDynamic(this, &UKratosLightAttackAbility::OnMontageBlendOut);
		AttackMontageTask->OnInterrupted.AddDynamic(this, &UKratosLightAttackAbility::OnMontageInterrupted);
		AttackMontageTask->OnCancelled.AddDynamic(this, &UKratosLightAttackAbility::OnMontageCancelled);
		AttackMontageTask->ReadyForActivation();

		//LaunchCharacterForwardSmoothly(CurComboCount);

	}
	else
	{
		Debug::Print(TEXT("UKratosLightAttackAbility::ExecuteAttackMontage - Failed creating montage task"), FColor::Orange);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosLightAttackAbility::SetPlayRateAttackMontage(int32 ComboCount, float PlayRate)
{
	UAnimInstance* AnimInstance = GetKratosFromActorInfo()->GetMesh()->GetAnimInstance();

	if (nullptr != AnimInstance && nullptr != LightAttackMontageMap[ComboCount])
	{
		AnimInstance->Montage_SetPlayRate(LightAttackMontageMap[ComboCount], PlayRate);
	}
}
