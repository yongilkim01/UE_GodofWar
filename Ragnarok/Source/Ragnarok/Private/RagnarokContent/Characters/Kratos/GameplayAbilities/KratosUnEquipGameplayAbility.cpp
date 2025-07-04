// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosUnEquipGameplayAbility.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Animation/KratosLinkedAnimLayer.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"

#include "EnhancedInputSubsystems.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/CombatSystem/CombatComponent.h"
#include "RagnarokEngine/GameItem/Weapon/RagnarokWeapon.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

void UKratosUnEquipGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (nullptr == UnEquipWeaponMontage)
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
		UnEquipWeaponMontage,
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
			WaitEventTask->EventReceived.AddDynamic(this, &UKratosUnEquipGameplayAbility::OnGameplayEventReceived);

			if (true == WaitEventTask->EventReceived.IsBound())
			{
				Debug::Print(TEXT("UnEquip Delegate Binding Sucess"));
			}
			else
			{
				Debug::Print(TEXT("UnEquip Delegate Binding Failed"));
			}

			WaitEventTask->ReadyForActivation();
		}
	}

	if (nullptr != MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UKratosUnEquipGameplayAbility::OnMontageCompleted);
		MontageTask->OnBlendOut.AddDynamic(this, &UKratosUnEquipGameplayAbility::OnMontageBlendOut);
		MontageTask->OnInterrupted.AddDynamic(this, &UKratosUnEquipGameplayAbility::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UKratosUnEquipGameplayAbility::OnMontageCancelled);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}

}

void UKratosUnEquipGameplayAbility::OnMontageCompleted()
{
	if (nullptr == WaitEventTask || false == WaitEventTask->IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosUnEquipGameplayAbility::OnMontageBlendOut()
{
	if (nullptr == WaitEventTask || false == WaitEventTask->IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosUnEquipGameplayAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosUnEquipGameplayAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosUnEquipGameplayAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	Debug::Print(TEXT("UnEquip"));

	AKratosWeapon* KratosWeapon = Cast<AKratosWeapon>(GetCombatComponentFromActorInfo()->GetCharacterWeaponByTag(UnEquipWeaponTag));

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
			EAttachmentRule::SnapToTarget,    // LocationRule
			EAttachmentRule::KeepRelative,    // RotationRule
			EAttachmentRule::KeepWorld,       // ScaleRule
			true						      // bWeldSimulatedBodies
		);

		KratosWeapon->AttachToComponent(ParentMesh, AttachRules, SocketNameToAttachTo);
	}

	{
		ParentMesh->UnlinkAnimClassLayers(KratosWeapon->WeaponData.WeaponAnimLayer.Get());
	}

	{
		ULocalPlayer* LocalPlayer = GetKratosControllerFromActorInfo()->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		InputSubsystem->RemoveMappingContext(KratosWeapon->WeaponData.InputMappingContext);
	}
	{
		GetASCFromActorInfo()->RemoveWeaponAbilities(KratosWeapon->GetGrantedAbilitySpecHandleArray());
	}

	{
		GetCombatComponentFromActorInfo()->CurrentEquippedWeaponTag = FGameplayTag::EmptyTag;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
