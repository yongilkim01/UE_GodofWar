// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosLightAttackAbility.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Items/Weapons/RagnarokWeapon.h"
#include "RagnarokContent/GameplayAbilities/RagnarokGameplayEffectContext.h"
#include "RagnarokContent/GameplayAbilities/AbilityTasks/RKAbilityTask_RotateToCamera.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"

#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokAbilityFunctionLibrary.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/CombatSystem/Tags/CombatGameplayTags.h"


UKratosLightAttackAbility::UKratosLightAttackAbility()
{
	CurComboCount = 1;
	bShowDebug = false;
}

void UKratosLightAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosLightAttackAbility::ActivateAbility"));

	Kratos->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	RotateToCameraTask = URKAbilityTask_RotateToCamera::CreateRotateToCameraTask(
		this,
		FName("RotateToCamera")
	);

	if (nullptr != RotateToCameraTask)
	{
		RotateToCameraTask->ReadyForActivation();
	}

	URagnarokFunctionLibrary::AddGameplayTagToActor(Kratos.Get(), KratosGameplayTags::Kratos_Status_Attacking);

	SetKratosAttackingState(true);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	bReserveComboAttack = false;

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
	KratosWeapon = Cast<AKratosWeapon>(GetCombatComponentFromActorInfo()->GetCurrentEquippedWeapon());
	Kratos->EquipWeaponToAttackSocekt(KratosWeapon);
	ExecuteAttackMontage(CurComboCount);
}

void UKratosLightAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosLightAttackAbility::EndAbility"));

	URagnarokFunctionLibrary::RemoveGameplayTagToActor(Kratos.Get(), KratosGameplayTags::Kratos_Status_Attacking);

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

	if (nullptr != RotateToCameraTask)
	{
		RotateToCameraTask->EndTask();
		RotateToCameraTask = nullptr;
	}

	bReserveComboAttack = false;
	SetKratosAttackingState(false);
	Kratos->UnEquipWeaponToAttackSocekt(KratosWeapon);
	CurComboCount = 1;
	URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), JumpTag);
}

void UKratosLightAttackAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosLightAttackAbility::InputPressed"));

	switch (CurAttackState)
	{
	case ERagnarokAttackState::ERAS_AttackWait:
		bReserveComboAttack = true;
		break;
	default:
		break;
	}
}

bool UKratosLightAttackAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (false == Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AKratos* KratosCharacter = Cast<AKratos>(ActorInfo->AvatarActor.Get());

	if (nullptr != KratosCharacter)
	{
		if (true == KratosCharacter->IsRunning() || 
			true == URagnarokFunctionLibrary::HasActorGameplayTag(ActorInfo->AvatarActor.Get(), KratosGameplayTags::Kratos_Status_Aiming))
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

void UKratosLightAttackAbility::OnMontageCompleted()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosLightAttackAbility::OnMontageCompleted"));
}

void UKratosLightAttackAbility::OnMontageBlendOut()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosLightAttackAbility::OnMontageBlendOut"));
}

void UKratosLightAttackAbility::OnMontageInterrupted()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosLightAttackAbility::OnMontageBlendOut"));
}

void UKratosLightAttackAbility::ResetAttackComboCount()
{
	CurComboCount = 1;
	URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), JumpTag);
	
}

void UKratosLightAttackAbility::ProcessNextCombo()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosLightAttackAbility::ProcessNextCombo"));

	if (nullptr != AttackMontageTask)
	{
		AttackMontageTask->EndTask();
		AttackMontageTask = nullptr;
	}

	CurComboCount++;

	if (MaxComboCount == CurComboCount)
	{
		URagnarokAbilityFunctionLibrary::AddGameplayTagToActor(GetKratosFromActorInfo(), JumpTag);
	}

	if (CurComboCount > MaxComboCount)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else
	{
		ExecuteAttackMontage(CurComboCount);
	}
}


void UKratosLightAttackAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	float WeaponDamage = GetKratosCombatComponent()->GetKratosEquippedWeaponDamageAtLevel(GetAbilityLevel());

	FGameplayEffectSpecHandle SpecHandle = CreateKratosComboDamageEffectSpecHandle(
		EffectClass,
		WeaponDamage,
		KratosGameplayTags::Kratos_SetByCaller_AttackType_Light,
		CurComboCount);

	AActor* TargetActor = const_cast<AActor*>(Payload.Target.Get());

	if (nullptr == TargetActor)
	{
		Debug::Print(TEXT("UKratosLightAttackAbility::OnGameplayEventReceived TargetActor is nullptr"), FColor::Red);
		return;
	}

	if (true == SpecHandle.IsValid())
	{
		FRagnarokGameplayEffectContext* Context = static_cast<FRagnarokGameplayEffectContext*>(SpecHandle.Data->GetContext().Get());
		if (nullptr != Context)
		{
			Context->SetAttackTypeTag(KratosGameplayTags::Kratos_AttackReact_Launch);
			Context->SetHitDirection((TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal());
		}
	}

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);
	
	FGameplayCueParameters CueParams;
	GetASCFromActorInfo()->ExecuteGameplayCue(
		FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Sounds.MeleeHit.Axe")),
		CueParams
	);

	if (true == ActiveGameplayEffectHandle.WasSuccessfullyApplied())
	{
		FGameplayEventData UpdatedPayload;
		UpdatedPayload.Target = Payload.Target;
		UpdatedPayload.Instigator = Payload.Instigator;

		if (true == SpecHandle.IsValid())
		{
			UpdatedPayload.ContextHandle = SpecHandle.Data->GetContext();
		}

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, RagnarokGameplayTags::Global_Event_HitReact, UpdatedPayload);
	}

	if (true == bShowDebug) Debug::Print(TEXT("Hitting ") + Payload.Target.GetName() + TEXT(" with light attack (Combo: ") + FString::FromInt(CurComboCount) + TEXT(")"), FColor::Cyan);
}

void UKratosLightAttackAbility::OnAttackWaitStartEventRecived(FGameplayEventData Payload)
{
	CurAttackState = ERagnarokAttackState::ERAS_AttackWait;
}

void UKratosLightAttackAbility::OnAttackWaitEndEventRecived(FGameplayEventData Payload)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosLightAttackAbility::OnAttackWaitEndEventRecived"));

	if (true == bReserveComboAttack)
	{
		bReserveComboAttack = false;
		ProcessNextCombo();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosLightAttackAbility::ExecuteAttackMontage(int32 ComboCount)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosLightAttackAbility::ExecuteAttackMontage"));

	if (nullptr == AttackMontage ||ComboCount > 3)
	{
		Debug::Print(TEXT("UKratosLightAttackAbility::ExecuteAttackMontage - Not contains combo section or attack montage is nullptr"), FColor::Red);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
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

	CurAttackState = ERagnarokAttackState::ERAS_Attacking;

	//FName StartSectionName = ComboSectionMap[ComboCount];
	FName StartSectionName = KratosAttackDataArray[ComboCount].ComboSectionName;

	AttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontage,
		1.0f,
		StartSectionName,
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

	if (nullptr != AnimInstance && nullptr != AttackMontage)
	{
		AnimInstance->Montage_SetPlayRate(AttackMontage, PlayRate);
	}
}
