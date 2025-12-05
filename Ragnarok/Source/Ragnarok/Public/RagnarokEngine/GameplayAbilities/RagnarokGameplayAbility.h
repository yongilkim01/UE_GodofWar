	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokGameplayAbility.generated.h"

class UCombatComponent;
class URagnarokAbilitySystemComponent;

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
	virtual bool DoesAbilitySatisfyTagRequirements(
		const UAbilitySystemComponent& AbilitySystemComponent, 
		const FGameplayTagContainer* SourceTags = nullptr, 
		const FGameplayTagContainer* TargetTags = nullptr, 
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~ End UGameplayAbility Interface.

	//~ Begin URagnarokGameplayAbility Interface.
	/** 어빌리티의 소유 액터를 카메라 방향으로 회전시키는 함수 */
	virtual void RotateOwnerToCameraDirection() {};
	//~ End URagnarokGameplayAbility Interface.

protected:
	void BreakAbility(
		const FString& ErrorMsg, 
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, bool bWasCancelled);

	UCombatComponent* GetCombatComponentFromActorInfo() const;
	URagnarokAbilitySystemComponent* GetASCFromActorInfo() const;
	FActiveGameplayEffectHandle ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

protected:
	UPROPERTY(EditAnywhere, Category = "GAS|Abilities")
	EAbilityActivationEvent AbilityActivationEvent = EAbilityActivationEvent::EAE_Triggered;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	bool bShowDebug = false;
	
};
