// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosRecallWeaponAbility.generated.h"

class AKratosWeapon;
class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosRecallWeaponAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

public:
	UKratosRecallWeaponAbility();

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

private:
	void PlayRecallAnimMontage();
	void RecallWeapon();
	bool IsPossibleRecallWeapon() const;

private:
	UPROPERTY(EditAnywhere, Category = "Ragnarok")
	UAnimMontage* RecallAnimMontage = nullptr;
	UPROPERTY()
	AKratosWeapon* CurWeapon = nullptr;

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = nullptr;

};
