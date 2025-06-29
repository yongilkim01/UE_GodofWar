// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosEquipWeaponGameplayAbility.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Animation/KratosLinkedAnimLayer.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Components/Combat/CombatComponent.h"
#include "RagnarokEngine/GameItem/Weapon/RagnarokWeapon.h"

void UKratosEquipWeaponGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (nullptr == EquipWeaponMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		EquipWeaponMontage,
		1.0f,
		NAME_None,
		false,
		0.0f,
		false
	);

	if (true == WaitForGameplayEventTag.IsValid())
	{
		WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			WaitForGameplayEventTag,
			nullptr,
			false,
			false
		);

		if (nullptr != WaitEventTask)
		{
			Debug::Print(TEXT("WaitEventTask Created"));

			// 바인딩 시도
			WaitEventTask->EventReceived.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnGameplayEventReceived);

			// 바인딩 성공 여부 확인
			if (WaitEventTask->EventReceived.IsBound())
			{
				Debug::Print(TEXT("Delegate Binding Success"));
			}
			else
			{
				Debug::Print(TEXT("Delegate Binding Failed"));
			}

			WaitEventTask->ReadyForActivation();
		}
	}


	if (nullptr != MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnMontageCompleted);
		MontageTask->OnBlendOut.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnMontageBlendOut);
		MontageTask->OnInterrupted.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnMontageCancelled);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UKratosEquipWeaponGameplayAbility::OnMontageCompleted()
{
	if (nullptr == WaitEventTask || false == WaitEventTask->IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosEquipWeaponGameplayAbility::OnMontageBlendOut()
{
	if (nullptr == WaitEventTask || false == WaitEventTask->IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosEquipWeaponGameplayAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosEquipWeaponGameplayAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosEquipWeaponGameplayAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	AKratosWeapon* KratosWeapon = Cast<AKratosWeapon>(GetCombatComponentFromActorInfo()->GetCharacterWeaponByTag(EquipWeaponTag));

	USkeletalMeshComponent* ParentMesh = nullptr;

	if (true == CurrentActorInfo->AvatarActor.IsValid())
	{
		APawn* Pawn = Cast<APawn>(CurrentActorInfo->AvatarActor.Get());

		if (nullptr != Pawn)
		{
			ParentMesh = Pawn->FindComponentByClass<USkeletalMeshComponent>();
		}
	}

	if (nullptr != ParentMesh)
	{
		FAttachmentTransformRules AttachRules(
			EAttachmentRule::KeepRelative,    // LocationRule
			EAttachmentRule::KeepRelative,    // RotationRule
			EAttachmentRule::KeepRelative,       // ScaleRule
			true						      // bWeldSimulatedBodies
		);

		KratosWeapon->AttachToComponent(
			ParentMesh,
			AttachRules,
			SocketNameToAttachTo // FName 타입의 멤버 변수여야 함
		);
	}

	ParentMesh->LinkAnimClassLayers(KratosWeapon->WeaponData.WeaponAnimLayer.Get());

	GetCombatComponentFromActorInfo()->CurrentEquippedWeaponTag = EquipWeaponTag;


	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}