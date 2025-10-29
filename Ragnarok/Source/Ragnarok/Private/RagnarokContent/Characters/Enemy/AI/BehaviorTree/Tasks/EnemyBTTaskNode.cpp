// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/AI/BehaviorTree/Tasks/EnemyBTTaskNode.h"
#include "RagnarokContent/Characters/Enemy/Base/EnemyCharacter.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "AIController.h"

UEnemyBTTaskNode::UEnemyBTTaskNode()
{
}

EBTNodeResult::Type UEnemyBTTaskNode::ExecuteEnemyTask(AEnemyCharacter* EnemyCharacter)
{
    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UEnemyBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    FEnemyTaskMemory* TaskMemory = (FEnemyTaskMemory*)NodeMemory;

    if (nullptr == TaskMemory->CachedEnemyCharacter)
    {
        if (AAIController* AIController = OwnerComp.GetAIOwner())
        {
            if (APawn* ControlledPawn = AIController->GetPawn())
            {
                AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(ControlledPawn);

                if (nullptr != EnemyCharacter)
                {
                    TaskMemory->CachedEnemyCharacter = EnemyCharacter;
                }
                else
                {
                    Debug::Print(TEXT("UEnemyBTTaskNode: Controlled Pawn is not an AEnemyCharacter."), FColor::Yellow);
                    return EBTNodeResult::Failed;
                }
            }
            else // ControlledPawn이 없음
            {
                Debug::Print(TEXT("UEnemyBTTaskNode: No Controlled Pawn found."), FColor::Yellow);
                return EBTNodeResult::Failed;
            }
        }
        else // AIController가 없음
        {
            Debug::Print(TEXT("UEnemyBTTaskNode: No AIController found."), FColor::Yellow);
            return EBTNodeResult::Failed;
        }
    }

    EBTNodeResult::Type Result = ExecuteEnemyTask(TaskMemory->CachedEnemyCharacter);

    return Result;
}