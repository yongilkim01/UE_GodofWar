// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosHitPauseGameplayAbility.generated.h"

class UKratosMeleeCameraShake;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosHitPauseGameplayAbility : public UKratosGameplayAbility
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
	TSubclassOf<UKratosMeleeCameraShake> CameraShakeClass;

private:
	const FGameplayAbilityActorInfo* HitPauseActorInfo = nullptr;
};
