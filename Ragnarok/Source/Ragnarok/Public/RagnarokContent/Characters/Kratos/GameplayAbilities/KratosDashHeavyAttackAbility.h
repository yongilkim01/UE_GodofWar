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

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~ End UGameplayAbility Interface.

private:
	UFUNCTION()
	void OnHitEventReceived(FGameplayEventData Payload);

	void ExecuteAttackMontage();
	void BeginMovement();
	void TickMovement();
	void EndMovement();

protected:
	//~ Begin UKratosGameplayAbility Class.
	virtual void OnMontageCompleted() override;
	virtual void OnMontageBlendOut() override;
	virtual void OnMontageInterrupted() override;
	virtual void OnMontageCancelled() override;
	//~ End UKratosGameplayAbility Class.

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	UAnimMontage* AttackMontageToPlay = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	TSubclassOf<UGameplayEffect> EffectClass;
	/** 이동 속도 커브 */
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UCurveFloat* MovementDurationCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	float AnimMontageRate = 1.6f;

private:
	/** 이동 관련 변수 */
	FVector StartLocation = FVector::ZeroVector;
	FVector TargetLocation = FVector::ZeroVector;
	FVector RollDirection = FVector::ZeroVector;
	float ElapsedTime = 0.0f;
	float Distance = 600.0f;
	float Duration = 1.5f;

	int AttackAbilityActiveCount;
	UAbilityTask_WaitGameplayEvent* HitWaitEventTask = nullptr;
	UAbilityTask_PlayMontageAndWait* AttackMontageTask = nullptr;
	FTimerHandle MovementTimerHandle;

};
