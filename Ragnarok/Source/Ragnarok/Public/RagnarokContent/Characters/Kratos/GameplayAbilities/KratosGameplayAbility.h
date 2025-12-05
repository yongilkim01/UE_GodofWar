// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokGameplayAbility.h"
#include "KratosGameplayAbility.generated.h"

class AKratos;
class AKratosController;
class UKratosCombatComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosGameplayAbility : public URagnarokGameplayAbility
{
	GENERATED_BODY()

public:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	//~ End UGameplayAbility Interface.

	//~ Begin UGameplayAbility Interface.
	virtual void RotateOwnerToCameraDirection() override;
	//~ End UGameplayAbility Interface.

public:
	AKratos* GetKratosFromActorInfo();
	AKratosController* GetKratosControllerFromActorInfo();
	UKratosCombatComponent* GetKratosCombatComponent();
	FGameplayEffectSpecHandle CreateKratosComboDamageEffectSpecHandle(
		TSubclassOf<UGameplayEffect> EffectClass,
		float Damage,
		FGameplayTag AttackTypeTag,
		int32 ComboCount
	);
	FGameplayEffectSpecHandle CreateKratosDamageEffectSpecHandle(
		TSubclassOf<UGameplayEffect> EffectClass,
		float Damage,
		FGameplayTag AttackTypeTag,
		float AddDamage = 1.0f
	);

protected:
	UFUNCTION()
	virtual void OnMontageCompleted();
	UFUNCTION()
	virtual void OnMontageBlendOut();
	UFUNCTION()
	virtual void OnMontageInterrupted();
	UFUNCTION()
	virtual void OnMontageCancelled();

	void SetKratosAttackingState(bool bAttacking);
	void SetKratosRollingState(bool bRolling);
	bool IsKratosRunning();

protected:
	TWeakObjectPtr<AKratos> Kratos;
	TWeakObjectPtr<AKratosController> KratosController;
};
