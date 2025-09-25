// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosThrowWeaponAbility.generated.h"

class AKratosWeapon;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosThrowWeaponAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

public:
	UKratosThrowWeaponAbility();

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

	virtual void InputPressed(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~ End UGameplayAbility Interface.

	//~ Begin UGameplayAbility Interface.
	virtual void OnMontageCompleted() override;
	//~ End UGameplayAbility Interface.

private:
	void ThrowWeapon();
	void PlayThrowAnimMontage();

	UFUNCTION()
	void OnThrowEventReceived(FGameplayEventData Payload);
	UFUNCTION()
	void OnHitEventReceived(FGameplayEventData Payload);

private:
	UPROPERTY(EditAnywhere)
	UAnimMontage* ThrowAnimMontage = nullptr;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> HitEffectClass;
	UPROPERTY()
	AKratosWeapon* CurWeapon = nullptr;

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = nullptr;
	UAbilityTask_WaitGameplayEvent* ThrowWaitEventTask = nullptr;
	UAbilityTask_WaitGameplayEvent* HitEventTask = nullptr;

};
