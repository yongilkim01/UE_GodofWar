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
	//~ End UGameplayAbility Interface.

protected:
	UFUNCTION()
	void OnResetAttackComboCount();
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnAttackMontageCompleted();

	UFUNCTION()
	void OnAttackWaitTimeOut();

	UFUNCTION()
	void OnNextComboInput();

	UFUNCTION()
	void StartAttackWaitState();
	void ExitAttackWaitState();
	void PauseMontageAtPosition(float Position);
	void ResumeMontage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	TMap<int, UAnimMontage*> LightAttackMontageMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	int32 CurComboCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	int32 UseComboCount = 1;
	
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
	TMap<int32, float> AttackWaitPositionMap;

	FTimerHandle TimerHandle;
	bool PrevbUseControllerRotationYaw;
	bool PrevbOrientRotationToMovement;
	int AttackAbilityActiveCount;
	UAbilityTask_WaitGameplayEvent* WaitEventTask = nullptr;
};
