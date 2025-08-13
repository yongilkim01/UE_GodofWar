// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosDashHeavyAttackAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosDashHeavyAttackAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

public:
	UKratosDashHeavyAttackAbility();

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
	//~ End UGameplayAbility Interface.

private:
	UFUNCTION()
	void OnHitEventReceived(FGameplayEventData Payload);

	void ExecuteAttackMontage();
	
protected:
	//~ Begin UKratosGameplayAbility Class.
	virtual void OnMontageCompleted() override;
	virtual void OnMontageBlendOut() override;
	virtual void OnMontageInterrupted() override;
	virtual void OnMontageCancelled() override;
	//~ End UKratosGameplayAbility Class.

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	UAnimMontage* AttackAnimMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	TSubclassOf<UGameplayEffect> EffectClass;

private:
	int AttackAbilityActiveCount;
	UAbilityTask_WaitGameplayEvent* HitWaitEventTask = nullptr;
	UAbilityTask_PlayMontageAndWait* AttackMontageTask = nullptr;
};
