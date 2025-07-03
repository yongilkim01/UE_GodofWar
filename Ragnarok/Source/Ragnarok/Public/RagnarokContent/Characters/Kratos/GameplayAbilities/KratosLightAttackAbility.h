// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosLightAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosLightAttackAbility : public UKratosGameplayAbility
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
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageBlendOut();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageCancelled();
	UFUNCTION()
	void OnResetAttackComboCount();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	TMap<int, UAnimMontage*> LightAttackMontageMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	int32 CurComboCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability")
	FGameplayTag JumpTag;

private:
	FTimerHandle TimerHandle;
	bool PrevbUseControllerRotationYaw;
	bool PrevbOrientRotationToMovement;
	int AttackAbilityActiveCount;
};
