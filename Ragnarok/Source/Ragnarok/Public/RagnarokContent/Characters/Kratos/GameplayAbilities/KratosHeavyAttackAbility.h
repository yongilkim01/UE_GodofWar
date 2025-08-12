// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
#include "KratosHeavyAttackAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosHeavyAttackAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

public:
	UKratosHeavyAttackAbility();

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
	//~ End UGameplayAbility Interface.

private:
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
	UFUNCTION()
	void OnAttackWaitStartEventRecived(FGameplayEventData Payload);
	UFUNCTION()
	void OnAttackWaitEndEventRecived(FGameplayEventData Payload);

	void ExecuteAttackMontage(int32 ComboCount);
	void ProcessNextCombo();
	void ResetAttackComboCount();

protected:
	//~ Begin Montage Callback Overrides
	virtual void OnMontageCompleted() override;
	virtual void OnMontageBlendOut() override;
	virtual void OnMontageInterrupted() override;
	virtual void OnMontageCancelled() override;
	//~ End Montage Callback Overrides

private:
	bool bReserveComboAttack = false;

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
	UAbilityTask_WaitGameplayEvent* HitWaitEventTask = nullptr;
	UAbilityTask_WaitGameplayEvent* AttackWaitStartTask = nullptr;
	UAbilityTask_WaitGameplayEvent* AttackWaitEndTask = nullptr;
	UAbilityTask_PlayMontageAndWait* AttackMontageTask = nullptr;
	ERagnarokAttackState CurAttackState = ERagnarokAttackState::ERAS_None;
};
