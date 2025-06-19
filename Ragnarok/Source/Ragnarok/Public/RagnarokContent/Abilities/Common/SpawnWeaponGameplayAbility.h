// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokGameplayAbility.h"
#include "SpawnWeaponGameplayAbility.generated.h"

class ARagnarokWeapon;

/**
 * 
 */
UCLASS()
class RAGNAROK_API USpawnWeaponGameplayAbility : public URagnarokGameplayAbility
{
	GENERATED_BODY()

public:
	USpawnWeaponGameplayAbility();

public:
	//~ Begin UGameplayAbility Interface.

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	//~ End UGameplayAbility Interface.

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName SocketNameToAttachTo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<ARagnarokWeapon> WeaponClass;

};
