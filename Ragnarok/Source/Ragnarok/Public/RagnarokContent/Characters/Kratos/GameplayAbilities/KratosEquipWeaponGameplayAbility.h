// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosEquipWeaponGameplayAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;

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
	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageBlendOut();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageCancelled();
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Animation")
	UAnimMontage* EquipWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Animation")
	FGameplayTag WaitForGameplayEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Weapon")
	FGameplayTag EquipWeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Weapon")
	FName SocketNameToAttachTo;

private:
	FGameplayAbilitySpecHandle CurrentSpecHandle;
	const FGameplayAbilityActorInfo* CurrentActorInfo;
	FGameplayAbilityActivationInfo CurrentActivationInfo;

	UAbilityTask_WaitGameplayEvent* WaitEventTask = nullptr;
};
