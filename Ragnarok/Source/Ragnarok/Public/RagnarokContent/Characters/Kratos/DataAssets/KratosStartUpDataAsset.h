// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/DataAssets/StartUpData/StartUpDataAsset.h"
#include "GameplayTagContainer.h"
#include "KratosStartUpDataAsset.generated.h"

USTRUCT()
struct FKratosAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ragnarok|InputTag")
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URagnarokGameplayAbility> Ability;

	bool IsVaild() const;
};

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosStartUpDataAsset : public UStartUpDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(
		URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ragnarok|StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FKratosAbilitySet> KratosStartUpAbilitySetArray;
	
};
