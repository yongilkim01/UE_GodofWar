// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosThrowWeaponAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"

UKratosThrowWeaponAbility::UKratosThrowWeaponAbility()
{
	bShowDebug = true;
}

void UKratosThrowWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::ActivateAbility"));

	CurWeapon = Cast<AKratosWeapon>(Kratos->GetKratosCombatComponent()->GetCurrentEquippedWeapon());

	if (nullptr == CurWeapon)
	{
		Debug::Print(TEXT("UKratosThrowWeaponAbility::CurWeapon is nullptr"), FColor::Red);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	Kratos->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	PlayThrowAnimMontage();
}

void UKratosThrowWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::EndAbility"));

	Kratos->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);


}

void UKratosThrowWeaponAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::InputPressed"));
}

void UKratosThrowWeaponAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::InputReleased"));
}

void UKratosThrowWeaponAbility::ThrowWeapon()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::ThrowWeapon"));

}

void UKratosThrowWeaponAbility::PlayThrowAnimMontage()
{
	if (nullptr == ThrowAnimMontage)
	{
		Debug::Print(TEXT("UKratosThrowWeaponAbility::ThrowAnimMontage is nullptr"), FColor::Red);
		return;
	}

	//PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	//	this,
	//	NAME_None,
	//	ThrowAnimMontage,
	//	1.0f,
	//	NAME_None,
	//	true,
	//	1.0f,
	//	0.0f
	//);

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		ThrowAnimMontage,
		1.0f,
		NAME_None,
		true,
		1.0f,
		0.037f,
		false
	);

	if (nullptr != PlayMontageTask)
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &UKratosThrowWeaponAbility::OnMontageCompleted);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &UKratosThrowWeaponAbility::OnMontageBlendOut);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UKratosThrowWeaponAbility::OnMontageInterrupted);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UKratosThrowWeaponAbility::OnMontageCancelled);
		PlayMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}
