// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);	

	UFUNCTION(BlueprintCallable, Category = "Ragnarok|Ability", meta = (ApplyLevel = "1"))
	void GrantWeaponAbilities(
		const TArray<FRagnarokbilitySet> InWeaponAbilityArray, 
		int32 ApplyLevel,
		TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandleArray);

	UFUNCTION(BlueprintCallable, Category = "Ragnarok|Ability")
	void RemoveWeaponAbilities(
		UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandleArray);

};
