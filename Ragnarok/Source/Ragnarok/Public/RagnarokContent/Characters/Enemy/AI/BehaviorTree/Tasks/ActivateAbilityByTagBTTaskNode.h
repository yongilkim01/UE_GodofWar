// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Enemy/AI/BehaviorTree/Tasks/EnemyBTTaskNode.h"
#include "GameplayTagContainer.h"
#include "ActivateAbilityByTagBTTaskNode.generated.h"

/**
 * 게임플레이 태그로 어빌리티를 활성화 하는 EnemyCharacter 전용 BTTaskNode 클래스
 */
UCLASS()
class RAGNAROK_API UActivateAbilityByTagBTTaskNode : public UEnemyBTTaskNode
{
	GENERATED_BODY()

public:
	/** nemyCharacter의 AbilitySystemComponent에서 TagToActivate에 해당하는 어빌리티를 활성화하는 함수 */
	virtual EBTNodeResult::Type ExecuteEnemyTask(AEnemyCharacter* EnemyCharacter) override;

protected:
	/** 활성화할 어빌리티의 게임플레이 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ActivateAbilityByTag|Ability", meta = (DisplayName = "Tag To Activate"))
	FGameplayTag TagToActivate;
};
