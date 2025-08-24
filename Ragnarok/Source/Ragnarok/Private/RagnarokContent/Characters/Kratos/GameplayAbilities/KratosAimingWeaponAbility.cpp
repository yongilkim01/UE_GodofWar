// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosAimingWeaponAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"
#include "RagnarokEngine/Systems/UISystem/RagnarokUserWidget.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"

UKratosAimingWeaponAbility::UKratosAimingWeaponAbility()
{
	bShowDebug = true;
}

void UKratosAimingWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosAimingWeaponAbility::ActivateAbility"));

	CurWeapon = Cast<AKratosWeapon>(Kratos->GetKratosCombatComponent()->GetCurrentEquippedWeapon());

	if (nullptr == CurWeapon)
	{
		Debug::Print(TEXT("UKratosAimingWeaponAbility::CurWeapon is nullptr"), FColor::Red);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	StartAiming();

}

void UKratosAimingWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosAimingWeaponAbility::EndAbility"));

	EndAiming();

}

void UKratosAimingWeaponAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosAimingWeaponAbility::InputPressed"));

	if (true == bAiming)
	{
		ThrowWeapon();
	}
}

void UKratosAimingWeaponAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosAimingWeaponAbility::InputReleased"));

	if (true == bAiming)
	{
		bAiming = false;
		URagnarokFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), KratosGameplayTags::Kratos_Status_Aiming);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
}

void UKratosAimingWeaponAbility::StartAiming()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosAimingWeaponAbility::StartAiming"));

	if (true == bAiming)
	{
		return;
	}

	bAiming = true;

	URagnarokFunctionLibrary::AddGameplayTagToActor(
		GetKratosFromActorInfo(), KratosGameplayTags::Kratos_Status_Aiming);

	if (AimWidgetClass && nullptr == AimWidget)
	{
		AimWidget = CreateWidget<URagnarokUserWidget>(GetWorld(), AimWidgetClass);
		if (nullptr != AimWidget)
		{
			AimWidget->AddToViewport();
		}
	}

}

void UKratosAimingWeaponAbility::EndAiming()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosAimingWeaponAbility::EndAiming"));

	URagnarokFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), KratosGameplayTags::Kratos_Status_Aiming);

	if (nullptr != AimWidget)
	{
		AimWidget->RemoveFromParent();
		AimWidget = nullptr;
	}
}

void UKratosAimingWeaponAbility::ThrowWeapon()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosAimingWeaponAbility::ThrowWeapon"));

}
