// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosDashLightAttackAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosDashLightAttackAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()
	
public:
	UKratosDashLightAttackAbility();

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

	//~ Begin UKratosGameplayAbility Class.
	virtual void OnMontageCompleted() override;
	virtual void OnMontageBlendOut() override;
	virtual void OnMontageInterrupted() override;
	virtual void OnMontageCancelled() override;
	//~ End UKratosGameplayAbility Class.

private:
	UFUNCTION()
	void OnHitEventReceived(FGameplayEventData Payload);

	void ExecuteAttackMontage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	UAnimMontage* AttackMontageToPlay = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	TSubclassOf<UGameplayEffect> EffectClass;
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	float AnimMontageRate = 1.6f;

private:

	UAbilityTask_WaitGameplayEvent* HitWaitEventTask = nullptr;
	UAbilityTask_PlayMontageAndWait* AttackMontageTask = nullptr;
};
