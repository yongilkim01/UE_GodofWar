// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Enemy/AI/BehaviorTree/Tasks/EnemyBTTaskNode.h"
#include "ToggleStrafingStateBTTaskNode.generated.h"

/**
 * 스트레이핑 상태를 활성화/비활성화하는 BTTaskNode 클래스
 */
UCLASS()
class RAGNAROK_API UToggleStrafingStateBTTaskNode : public UEnemyBTTaskNode
{
	GENERATED_BODY()

public:
	/** 적 캐릭터의 스트레이핑 상태를 bEnableStrafing 변수에 따라서 업데이트 하는 함수 */
	virtual EBTNodeResult::Type ExecuteEnemyTask(AEnemyCharacter* EnemyCharacter) override;

protected:
	/** 스트레이핑을 활성화 여부 변수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ToggleStrafingState|Settings", meta = (DisplayName = "Enable Strafing"))
	bool bEnableStrafing = true;

	/** 스트레이핑 활성화 시 이동 속도를 변경할지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ToggleStrafingState|Settings", meta = (DisplayName = "Change Max Walk Speed"))
	bool bChangeMaxWalkSpeed = true;

	/** 스트레이핑 중 이동 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ToggleStrafingState|Movement", meta = (DisplayName = "Strafing Walk Speed"))
	float StrafingWalkSpeed = 200.0f;

	/** 복원할 때 사용하는 블랙보드의 기본 최대 이동 속도 키 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ToggleStrafingState|Blackboard", meta = (DisplayName = "Default Max Walk Speed Key"))
	FBlackboardKeySelector DefaultMaxWalkSpeedKey;
};
