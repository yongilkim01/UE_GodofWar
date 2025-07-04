// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokGameplayAbility.h"
#include "EnemyGameplayAbility.generated.h"

class AEnemyCharacter;
class UEnemyCombatComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UEnemyGameplayAbility : public URagnarokGameplayAbility
{
	GENERATED_BODY()

public:
	AEnemyCharacter* GetEnemyCharacterFromActorInfo();
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<AEnemyCharacter> EnemyCharacter;
};
