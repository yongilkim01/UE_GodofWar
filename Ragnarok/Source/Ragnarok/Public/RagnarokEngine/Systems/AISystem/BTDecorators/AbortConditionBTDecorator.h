// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTDecorator.h"
#include "AbortConditionBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UAbortConditionBTDecorator : public URagnarokBTDecorator
{
	GENERATED_BODY()

public:
	UAbortConditionBTDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI|AbortCondition")
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI|AbortCondition")
	FBlackboardKeySelector DistanceToTargetKey;

};
