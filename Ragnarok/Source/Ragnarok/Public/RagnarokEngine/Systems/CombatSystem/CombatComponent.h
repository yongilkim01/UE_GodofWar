// Fill out your copyrighnotice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/Components/RagnarokActorComponent.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
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

	UFUNCTION()
	void ToggleWeaponCollision(bool bCollisionEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::ETD_CurrentEquippedWeapon);
	
	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

private:
	TMap<FGameplayTag, ARagnarokWeapon*> CharacterWeaponMap;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Combat")
	FGameplayTag CurrentEquippedWeaponTag = FGameplayTag::EmptyTag;
};
