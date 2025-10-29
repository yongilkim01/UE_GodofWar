// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTService.h"
#include "MotionWrapAttackTargetBTService.generated.h"

class AEnemyCharacter;

/**
 * 블랙보드에서 TargetActor를 가져와서 Owner의 모션 워핑 타겟을 업데이트하는 BTService 클래스
 */
UCLASS()
class RAGNAROK_API UMotionWrapAttackTargetBTService : public URagnarokBTService
{
	GENERATED_BODY()

public:
	/** 생성자 */
	UMotionWrapAttackTargetBTService();

	/** 블랙보드에서 TargetActor를 가져와 Owner의 모션 워핑 컴포넌트의 AttackTarget 으로 등록 */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	/** 블랙보드의 TargetActor 키 */
	UPROPERTY(EditAnywhere, Category = "MotionWrapAttackTarget|Blackboard", meta = (DisplayName = "Target Actor Key"))
	FBlackboardKeySelector TargetActorKey;

private:
	/** AI 컨트롤러가 제어하는 적 캐릭터 레퍼런스 */
	AEnemyCharacter* EnemyCharacter = nullptr;
};
