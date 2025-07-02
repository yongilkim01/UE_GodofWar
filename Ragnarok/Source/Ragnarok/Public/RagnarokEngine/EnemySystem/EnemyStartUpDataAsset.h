// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/DataAssets/StartUpData/StartUpDataAsset.h"
#include "EnemyStartUpDataAsset.generated.h"

class URagnarokAbilitySystemComponent;
class UEnemyGameplayAbility;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UEnemyStartUpDataAsset : public UStartUpDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(
		URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ragnarok|StartUpData")
	TArray<TSubclassOf<UEnemyGameplayAbility>> EnemyCombatAbilityArray;
};
