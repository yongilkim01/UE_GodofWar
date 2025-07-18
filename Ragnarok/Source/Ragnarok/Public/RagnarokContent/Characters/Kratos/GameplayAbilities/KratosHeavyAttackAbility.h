// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosHeavyAttackAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosHeavyAttackAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

public:
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
	void OnResetAttackComboCount();
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok")
	TMap<int, UAnimMontage*> HeavyAttackMontageMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok")
	int32 CurComboCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	int32 UseComboCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	FGameplayTag JumpTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	TSubclassOf<UGameplayEffect> EffectClass;

private:
	FTimerHandle TimerHandle;
	int AttackAbilityActiveCount;
	UAbilityTask_WaitGameplayEvent* WaitEventTask = nullptr;
};
