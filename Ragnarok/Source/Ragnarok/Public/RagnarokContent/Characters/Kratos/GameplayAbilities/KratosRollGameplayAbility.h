// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
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

protected:
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

	virtual void InputPressed(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~ End UGameplayAbility Interface.

	//~ Begin UKratosGameplayAbility Interface.
	virtual void OnMontageCompleted() override;
	virtual void OnMontageBlendOut() override;
	virtual void OnMontageInterrupted() override;
	//~ End UKratosGameplayAbility Interface.

private:
	void CalcAndPlayAnimMontage();
	void CalcAnimMontage();
	void PlayRollAnimMontage();

private:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	TMap<ERagnarokDirection, UAnimMontage*> DodgeAnimMontageMap;
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	TMap<ERagnarokDirection, UAnimMontage*> RollAnimMontageMap;

private:
	UAbilityTask_PlayMontageAndWait* RollMontageTask;
	UAnimMontage* RollAnimMontage = nullptr;
	ERagnarokRollState CurRollState = ERagnarokRollState::ERRS_None;
	FVector RollDirection = FVector::ZeroVector;
};
