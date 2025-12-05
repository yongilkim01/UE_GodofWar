// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosEquipWeaponGameplayAbility.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Animation/KratosLinkedAnimLayer.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosUIComponent.h"

#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpecHandle.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/CombatSystem/CombatComponent.h"
#include "RagnarokContent/Items/Weapons/RagnarokWeapon.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokContent/UI/Components/RagnarokUIComponent.h"

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
			WaitEventTask->EventReceived.AddDynamic(this, &UKratosEquipWeaponGameplayAbility::OnGameplayEventReceived);
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
			SocketNameToAttachTo // FName Ÿ���� ��� �������� ��
		);
	}

	{
		ParentMesh->LinkAnimClassLayers(KratosWeapon->WeaponData.WeaponAnimLayer.Get());
	}


	{
		ULocalPlayer* LocalPlayer = GetKratosControllerFromActorInfo()->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		InputSubsystem->AddMappingContext(KratosWeapon->WeaponData.InputMappingContext, 1);

		TArray<FGameplayAbilitySpecHandle> SpecHandleArray;

		GetASCFromActorInfo()->GrantWeaponAbilities(
			KratosWeapon->WeaponData.WeaponAbilitySetArray,
			GetAbilityLevel(),
			SpecHandleArray);

		KratosWeapon->AssignGratnAbilitySpecHandles(SpecHandleArray);
	}
	{
		GetCombatComponentFromActorInfo()->CurrentEquippedWeaponTag = EquipWeaponTag;
	}
	{
		UKratosUIComponent* KratosUIComponent = Cast<UKratosUIComponent>(GetKratosFromActorInfo()->GetUIComponent());

		if (nullptr != KratosUIComponent)
		{
			KratosUIComponent->OnEquippedWeaponChanged.Broadcast(KratosWeapon->WeaponData.WeaponIconTextureSoftPtr);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}