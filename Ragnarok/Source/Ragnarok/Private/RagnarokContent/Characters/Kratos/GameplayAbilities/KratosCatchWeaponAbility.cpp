// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosCatchWeaponAbility.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

UKratosCatchWeaponAbility::UKratosCatchWeaponAbility()
{
	bShowDebug = true;

}

void UKratosCatchWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosCatchWeaponAbility::ActivateAbility"));

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


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
