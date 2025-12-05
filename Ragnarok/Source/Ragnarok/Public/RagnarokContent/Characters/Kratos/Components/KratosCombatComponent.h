// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/CombatSystem/CombatComponent.h"
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

	AKratosWeapon* GetKratosEquippedWeapopn() const;
	float GetKratosEquippedWeaponDamageAtLevel(float Level) const;

	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};
