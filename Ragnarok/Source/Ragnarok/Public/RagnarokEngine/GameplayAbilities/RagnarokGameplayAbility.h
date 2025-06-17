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
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
	//~ End UGameplayAbility Interface.

protected:
	UPROPERTY(EditAnywhere, Category = "GAS|Abilities")
	EAbilityActivationEvent AbilityActivationEvent = EAbilityActivationEvent::EAE_Triggered;
	
};
