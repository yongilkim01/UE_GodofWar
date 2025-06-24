// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Components/RagnarokActorComponent.h"
#include "GameplayTagContainer.h"
#include "CombatComponent.generated.h"

class ARagnarokWeapon;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UCombatComponent : public URagnarokActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void RegisterSpawnWeapon(
		FGameplayTag InRegisterWeaponTag, 
		ARagnarokWeapon* InRegisterWeapon,
		bool bEquipWeapon = false);

	UFUNCTION()
	ARagnarokWeapon* GetCharacterWeaponByTag(FGameplayTag InFindWeaponTag) const;

	UFUNCTION()
	ARagnarokWeapon* GetCurrentEquippedWeapon() const;
	
private:
	TMap<FGameplayTag, ARagnarokWeapon*> CharacterWeaponMap;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Combat")
	FGameplayTag CurrentEquippedWeaponTag;
};
