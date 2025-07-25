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
	void ComputeRollDirectionAndDistance();

private:

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	FRotator PrevRotator = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	FName WarpTargetName;

	UPROPERTY(EditAnywhere, Category = "Kratos|Ability")
	UAnimMontage* RollingAnimMontage = nullptr;

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

	UPROPERTY(EditAnywhere, Category = "Roll")
	float RollDistance = 1000.f; // 구르기 거리 (예: 400cm)

private:
	FVector RollingDirection = FVector::ZeroVector;
	
};
