// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/AI/BehaviorTree/RagnarokBTDecorator.h"
#include "GameplayTagContainer.h"
#include "HasGameplayTagBTDecorator.generated.h"

/**
 * 게임플레이 태그 보유 체크 BTDecorator 클래스
 */
UCLASS()
class RAGNAROK_API UHasGameplayTagBTDecorator : public URagnarokBTDecorator
{
	GENERATED_BODY()

public:
	/** 생성자 */
	UHasGameplayTagBTDecorator();

protected:
	/** 블랙보드에서 CheckActor를 가져와 TagToCheck를 가지고 있는지 확인하는 함수 */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	/** 블랙보드에서 체크할 액터의 키 */
	UPROPERTY(EditAnywhere, Category = "HasGameplayTag|Blackboard", meta = (DisplayName = "Actor To Check Key"))
	FBlackboardKeySelector CheckActorKey;
	/** 체크할 게임플레이 태그 */
	UPROPERTY(EditAnywhere, Category = "HasGameplayTag|Tag", meta = (DisplayName = "Tag To Check"))
	FGameplayTag TagToCheck;
	/** 조건을 반전시킬지 여부 */
	UPROPERTY(EditAnywhere, Category = "HasGameplayTag|Tag", meta = (DisplayName = "Inverse Condition Check"))
	bool bInverseConditionCheck = false;
};
