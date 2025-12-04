// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/AI/BehaviorTree/Decorators/ComputeSucessChanceBTDecorator.h"
#include "AIController.h"

bool UComputeSucessChanceBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (nullptr == AIController)
    {
        return false;
    }

    APawn* OwnerPawn = AIController->GetPawn();
    if (nullptr == OwnerPawn)
    {
        return false;
    }

    return FMath::FRand() < FMath::FRandRange(SuccessChanceMin, SuccessChanceMax);
}
