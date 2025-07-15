// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTDecorators/ComputeSucessChanceBTDecorator.h"
#include "AIController.h"

bool UComputeSucessChanceBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (nullptr == AIController)
    {
        return false;
    }

    APawn* ControlledPawn = AIController->GetPawn();
    if (nullptr == ControlledPawn)
    {
        return false;
    }

    return FMath::FRand() < FMath::FRandRange(SuccessChanceMin, SuccessChanceMax);
}
