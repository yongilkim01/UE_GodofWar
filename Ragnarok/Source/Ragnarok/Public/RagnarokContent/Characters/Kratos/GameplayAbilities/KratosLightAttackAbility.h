// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
#include "KratosLightAttackAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosLightAttackAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

public:
	UKratosLightAttackAbility();

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

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr, 
		const FGameplayTagContainer* TargetTags = nullptr, 
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	//~ End UGameplayAbility Interface.

	//~ Begin UKratosGameplayAbility Interface.
	virtual void OnMontageCompleted() override;
	virtual void OnMontageBlendOut() override;
	virtual void OnMontageInterrupted() override;
	//~ End UKratosGameplayAbility Interface.

protected:
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
	UFUNCTION()
	void OnAttackWaitStartEventRecived(FGameplayEventData Payload);
	UFUNCTION()
	void OnAttackWaitEndEventRecived(FGameplayEventData Payload);

private:
	void ExecuteAttackMontage(int32 ComboCount);
	void SetPlayRateAttackMontage(int32 ComboCount, float PlayRate);
	void ResetAttackComboCount();
	void ProcessNextCombo();
	void LaunchCharacterForward(int32 ComboCount);
	void LaunchCharacterForwardSmoothly(int32 ComboCount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	TMap<int, UAnimMontage*> LightAttackMontageMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	int32 CurComboCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	FGameplayTag JumpTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	ERagnarokAttackState CurAttackState = ERagnarokAttackState::ERAS_None;

private:
	UAbilityTask_PlayMontageAndWait* AttackMontageTask = nullptr;
	FTimerHandle AttackWaitTimerHandle;
	float ComboWaitDuration = 1.0f;

	FTimerHandle TimerHandle;
	UAbilityTask_WaitGameplayEvent* WaitEventTask = nullptr;
	UAbilityTask_WaitGameplayEvent* AttackWaitStartTask = nullptr;
	UAbilityTask_WaitGameplayEvent* AttackWaitEndTask = nullptr;
	bool bReserveComboAttack = false;

	TMap<int32, float> LaunchPowerMap;
};
