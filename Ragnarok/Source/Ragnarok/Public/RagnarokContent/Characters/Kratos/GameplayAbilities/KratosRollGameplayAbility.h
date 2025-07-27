// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosRollGameplayAbility.generated.h"

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

private:
	UFUNCTION()
	void OnDelayFinished();

	UFUNCTION()
	void OnResetEvasion();

	UFUNCTION()
	void ComputeDodgeDirection();

	UFUNCTION()
	void ComputeRollingDirection();

private:

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	FRotator PrevRotator = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	FName WarpTargetName;

private:
	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* AbilityAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* DodgeForwardAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* DodgeRightAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* DodgeLeftAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* DodgeBackwardAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* DodgeRFAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* DodgeLFAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* DodgeRBAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* DodgeLBAnimMontage = nullptr;


private:
	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* RollingForwardAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* RollingRightAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* RollingLeftAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* RollingBackwardAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* RollingRFAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* RollingLFAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* RollingRBAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* RollingLBAnimMontage = nullptr;

private:
	float RollDistance = 1000.f;
	FVector RollingDirection = FVector::ZeroVector;
	FTimerHandle TimerHandle;
	bool bEvasion = false;
};
