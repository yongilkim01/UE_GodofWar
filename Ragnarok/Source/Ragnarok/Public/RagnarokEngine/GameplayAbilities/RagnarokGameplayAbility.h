	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void OnGiveAbility(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

protected:
	void BreakAbility(
		const FString& ErrorMsg, 
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, bool bWasCancelled);

protected:
	UPROPERTY(EditAnywhere, Category = "GAS|Abilities")
	EAbilityActivationEvent AbilityActivationEvent = EAbilityActivationEvent::EAE_Triggered;
	
};
