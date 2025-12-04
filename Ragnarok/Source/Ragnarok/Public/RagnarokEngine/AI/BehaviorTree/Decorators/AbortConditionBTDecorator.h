// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/AI/BehaviorTree/RagnarokBTDecorator.h"
#include "AbortConditionBTDecorator.generated.h"

/**
 * AI 행동 중단 체크 BTDecorator 클래스
 */
UCLASS()
class RAGNAROK_API UAbortConditionBTDecorator : public URagnarokBTDecorator
{
	GENERATED_BODY()

public:
	/** 생성자 */
	UAbortConditionBTDecorator();

protected:
	/** 타겟이 죽었거나, AI가 죽었거나, 타겟과의 거리가 0이면 true를 반환하여 행동을 중단시키는 함수 */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	/** 블랙보드의 타겟 액터 키 */
	UPROPERTY(EditAnywhere, Category = "AbortCondition|Blackboard", meta = (DisplayName = "Target Actor Key"))
	FBlackboardKeySelector TargetActorKey;

	/** 블랙보드의 타겟까지의 거리 키 */
	UPROPERTY(EditAnywhere, Category = "AbortCondition|Blackboard", meta = (DisplayName = "Distance To Target Key"))
	FBlackboardKeySelector DistanceToTargetKey;
};
