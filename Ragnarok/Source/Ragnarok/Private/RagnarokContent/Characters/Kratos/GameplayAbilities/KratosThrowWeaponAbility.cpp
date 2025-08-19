// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosThrowWeaponAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"

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

	StartAiming();

}

void UKratosThrowWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::EndAbility"));


	if (true == bAiming)
	{
		EndAiming();
	}

}

void UKratosThrowWeaponAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::InputPressed"));

	if (true == bAiming)
	{
		ThrowWeapon();
	}
}

void UKratosThrowWeaponAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::InputReleased"));

	if (true == bAiming)
	{
		bAiming = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
}

void UKratosThrowWeaponAbility::StartAiming()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::StartAiming"));

	if (true == bAiming)
	{
		return;
	}

	bAiming = true;

	URagnarokFunctionLibrary::AddGameplayTagToActor(
		GetKratosFromActorInfo(), KratosGameplayTags::Kratos_Status_Aiming);

}

void UKratosThrowWeaponAbility::EndAiming()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::EndAiming"));

	URagnarokFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), KratosGameplayTags::Kratos_Status_Aiming);

}

void UKratosThrowWeaponAbility::ThrowWeapon()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::ThrowWeapon"));

}
