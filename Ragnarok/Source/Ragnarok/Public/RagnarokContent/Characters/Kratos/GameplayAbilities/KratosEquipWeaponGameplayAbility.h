// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosEquipWeaponGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosEquipWeaponGameplayAbility : public UKratosGameplayAbility
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

private:
	void OnMontageCompleted();
	void OnMontageBlendOut();
	void OnMontageInterrupted();
	void OnMontageCancelled();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Animation")
	UAnimMontage* EquipWeaponMontage;

private:
	FGameplayAbilitySpecHandle CurrentSpecHandle;
	const FGameplayAbilityActorInfo* CurrentActorInfo;
	FGameplayAbilityActivationInfo CurrentActivationInfo;
	
};
