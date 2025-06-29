// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Components/Combat/CombatComponent.h"
#include "KratosCombatComponent.generated.h"

class AKratosWeapon;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ragnarok|Combat")
	AKratosWeapon* GetKratosWeaponByTag(FGameplayTag InFindWeaponTag) const;
};
