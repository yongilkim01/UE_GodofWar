// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosUnEquipGameplayAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosUnEquipGameplayAbility : public UKratosGameplayAbility
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

protected:
	virtual void OnMontageCompleted() override;
	virtual void OnMontageBlendOut() override;

	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Animation")
	UAnimMontage* UnEquipWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Animation")
	FGameplayTag WaitForGameplayEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Weapon")
	FGameplayTag UnEquipWeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Weapon")
	FName SocketNameToAttachTo;

private:
	UAbilityTask_WaitGameplayEvent* WaitEventTask = nullptr;

};
