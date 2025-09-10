// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosCatchWeaponAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;
class AKratosWeapon;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosCatchWeaponAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

public:
	UKratosCatchWeaponAbility();

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~ End UGameplayAbility Interface.

private:
	void PlayCatchAnimation();
	void TriggerCameraShake();
	void AttachWeaponToKratos();

	UFUNCTION()
	void OnCatchAnimationComplete();

private:
	UPROPERTY(EditAnywhere)
	UAnimMontage* CatchAnimMontage = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CatchCameraShake = nullptr;

	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = nullptr;

	UPROPERTY()
	AKratosWeapon* CatchingWeapon = nullptr;

};
