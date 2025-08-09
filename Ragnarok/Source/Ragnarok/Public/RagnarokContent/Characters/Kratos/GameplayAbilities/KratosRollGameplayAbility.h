// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosRollGameplayAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosRollGameplayAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

public:
	UKratosRollGameplayAbility();

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

	//~ Begin UKratosGameplayAbility Interface.
	virtual void OnMontageCompleted() override;
	virtual void OnMontageBlendOut() override;
	virtual void OnMontageInterrupted() override;
	//~ End UKratosGameplayAbility Interface.

private:
	UFUNCTION()
	void OnResetEvasion();

	UFUNCTION()
	void ComputeDodgeDirection();

	UFUNCTION()
	void ComputeRollingDirection();

private:
	void BeginSmoothMovement();
	void TickSmoothMovement();
	void EndSmmothMovement();

private:

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	FRotator PrevRotator = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	FName WarpTargetName;

private:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* DodgeForwardAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* DodgeRightAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* DodgeLeftAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* DodgeBackwardAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* DodgeRFAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* DodgeLFAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* DodgeRBAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* DodgeLBAnimMontage = nullptr;


private:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* RollingForwardAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* RollingRightAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* RollingLeftAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* RollingBackwardAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* RollingRFAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* RollingLFAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* RollingRBAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UAnimMontage* RollingLBAnimMontage = nullptr;

private:
	/** 회피 거리 */
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	float DodgeDistance = 200.0f;

	/** 회피 거리 */
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	float RollDistance = 300.f;

	/** 이동 시간 */
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	float DodgeMovementDuration = 0.2f;

	/** 이동 시간 */
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	float RollMovementDuration = 0.4f;

	/** 이동 속도 커브 */
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	UCurveFloat* MovementDurationCurve;

	/** 이동 관련 변수 */
	FVector StartLocation = FVector::ZeroVector;
	FVector TargetLocation = FVector::ZeroVector;
	float ElapsedTime = 0.0;
	FTimerHandle MovementTimerHandle;

	FVector RollDirection = FVector::ZeroVector;
	FTimerHandle TimerHandle;
	bool bEvasion = false;

	UAbilityTask_PlayMontageAndWait* RollMontageTask;
	UAnimMontage* RollAnimMontage = nullptr;
};
