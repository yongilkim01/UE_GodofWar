// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosDrawOverlayWidgetAbility.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosDrawOverlayWidgetAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	//~ End UGameplayAbility Interface.

protected:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability")
	TSubclassOf<UUserWidget> WidgetClass;
	
};
