// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Enemy/Abilities/EnemyGameplayAbility.h"
#include "EnemyHitReactGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UEnemyHitReactGameplayAbility : public UEnemyGameplayAbility
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

protected:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	TArray<UAnimMontage*> AnimMontageArray;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	bool bHasHitReactMontagesToPlay = true;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	bool bRotateToATargetActor = true;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	TSubclassOf<UGameplayEffect> HitEffectClass;
};
