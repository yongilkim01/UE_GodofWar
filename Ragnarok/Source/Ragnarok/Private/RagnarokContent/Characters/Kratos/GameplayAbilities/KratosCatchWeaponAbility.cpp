// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosCatchWeaponAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UKratosCatchWeaponAbility::UKratosCatchWeaponAbility()
{
	bShowDebug = true;

}

void UKratosCatchWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosCatchWeaponAbility::ActivateAbility"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (nullptr != TriggerEventData && nullptr != TriggerEventData->Target)
	{
		CatchingWeapon = Cast<AKratosWeapon>(Kratos->GetKratosCombatComponent()->GetCurrentEquippedWeapon());
	}

	PlayCatchAnimation();
	TriggerCameraShake();

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
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepRelative,
			true			  
		);

		CatchingWeapon->AttachToComponent(
			ParentMesh,
			AttachRules,
			FName("RightWeaponSocket")
		);
	}

	Kratos->ShakeCamera();
	CatchingWeapon->EndWeaponTrail();

}

void UKratosCatchWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosCatchWeaponAbility::EndAbility"));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


}

bool UKratosCatchWeaponAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosCatchWeaponAbility::CanActivateAbility"));

	if (false == Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UKratosCatchWeaponAbility::PlayCatchAnimation()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosCatchWeaponAbility::PlayCatchAnimation"));

	if (nullptr == CatchAnimMontage)
	{
		Debug::Print(TEXT("UKratosRecallWeaponAbility::CatchAnimMontage is nullptr"), FColor::Red);
		return;
	}

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		CatchAnimMontage,
		1.0f,
		NAME_None,
		true
	);

	if (nullptr != PlayMontageTask)
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &UKratosCatchWeaponAbility::OnMontageCompleted);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &UKratosCatchWeaponAbility::OnMontageBlendOut);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UKratosCatchWeaponAbility::OnMontageInterrupted);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UKratosCatchWeaponAbility::OnMontageCancelled);

		PlayMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

}

void UKratosCatchWeaponAbility::TriggerCameraShake()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosCatchWeaponAbility::TriggerCameraShake"));

}

void UKratosCatchWeaponAbility::AttachWeaponToKratos()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosCatchWeaponAbility::AttachWeaponToKratos"));

}

void UKratosCatchWeaponAbility::OnCatchAnimationComplete()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosCatchWeaponAbility::OnCatchAnimationComplete"));

}

void UKratosCatchWeaponAbility::OnNotifyBegin()
{
}

void UKratosCatchWeaponAbility::OnNotifyEnd()
{
}
