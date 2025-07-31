// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/AI/BehaviorTree/Tasks/EnemyBTTaskNode.h"
#include "RagnarokEngine/Objects/Characters/Base/EnemyCharacter.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

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
    FEnemyTaskMemory* MyMemory = (FEnemyTaskMemory*)NodeMemory;

    if (nullptr == MyMemory->CachedEnemyCharacter)
    {
        if (AAIController* AIController = OwnerComp.GetAIOwner())
        {
            if (APawn* ControlledPawn = AIController->GetPawn())
            {
                AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(ControlledPawn);

                if (nullptr != EnemyCharacter)
                {
                    MyMemory->CachedEnemyCharacter = EnemyCharacter;
                }
                else
                {
                    Debug::Print(TEXT("UEnemyBTTaskNode: Controlled Pawn is not an AEnemyCharacter."), FColor::Yellow);
                    return EBTNodeResult::Failed;
                }
            }
            else // ControlledPawn�� ����
            {
                Debug::Print(TEXT("UEnemyBTTaskNode: No Controlled Pawn found."), FColor::Yellow);
                return EBTNodeResult::Failed;
            }
        }
        else // AIController�� ����
        {
            Debug::Print(TEXT("UEnemyBTTaskNode: No AIController found."), FColor::Yellow);
            return EBTNodeResult::Failed;
        }
    }

    EBTNodeResult::Type Result = ExecuteEnemyTask(MyMemory->CachedEnemyCharacter);

    return Result;
}