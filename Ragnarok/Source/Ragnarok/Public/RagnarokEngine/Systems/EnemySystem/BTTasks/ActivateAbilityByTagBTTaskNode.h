// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyBTTaskNode.h"
#include "ActivateAbilityByTagBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UActivateAbilityByTagBTTaskNode : public UEnemyBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteEnemyTask(AEnemyCharacter* EnemyCharacter) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability|Weapon")
	FGameplayTag TagToActivate;
};
