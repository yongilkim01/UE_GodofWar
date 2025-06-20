// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/DataAssets/RagnarokDataAsset.h"
#include "StartUpDataAsset.generated.h"

class URagnarokGameplayAbility;
class URagnarokAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UStartUpDataAsset : public URagnarokDataAsset
{
	GENERATED_BODY()

public:
	//~ Begin UStartUpDataAsset Interface.
	virtual void GiveToAbilitySystemComponent(
		URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel = 1);
	//~ End UStartUpDataAsset Interface.

protected:
	void GrantAbilities(
		const TArray< TSubclassOf<URagnarokGameplayAbility>>& InGrantAbilityArray,
		URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditAnywhere, Category = "StartUpData|Ability")
	TArray<TSubclassOf<URagnarokGameplayAbility>> ActivateOnGivenAbilityArray;

	UPROPERTY(EditAnywhere, Category = "StartUpData|Ability")
	TArray<TSubclassOf<URagnarokGameplayAbility>> ReactiveAbilityArray;
};
