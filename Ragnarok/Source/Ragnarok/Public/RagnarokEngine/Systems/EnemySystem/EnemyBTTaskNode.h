// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTTaskNode.h"
#include "EnemyBTTaskNode.generated.h"

class AEnemyCharacter;

struct FEnemyTaskMemory
{
	AEnemyCharacter* CachedEnemyCharacter; // 여기에 OwningEnemyCharacter를 저장
};

/**
 * 
 */
UCLASS()
class RAGNAROK_API UEnemyBTTaskNode : public URagnarokBTTaskNode
{
	GENERATED_BODY()

public:
	UEnemyBTTaskNode();

	virtual EBTNodeResult::Type ExecuteEnemyTask(AEnemyCharacter* EnemyCharacter);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FEnemyTaskMemory); }

};
