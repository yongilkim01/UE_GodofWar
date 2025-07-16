// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTService.h"
#include "MotionWrapAttackTargetBTService.generated.h"

class AEnemyCharacter;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UMotionWrapAttackTargetBTService : public URagnarokBTService
{
	GENERATED_BODY()

public:
	UMotionWrapAttackTargetBTService();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetActorKey;

private:
	AEnemyCharacter* EnemyCharacter = nullptr;
};
