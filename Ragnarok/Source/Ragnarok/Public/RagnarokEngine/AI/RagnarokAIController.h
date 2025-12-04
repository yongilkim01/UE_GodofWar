// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RagnarokAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

/**
 * Ragnarok AI Controller 베이스 클래스.
 */
UCLASS()
class RAGNAROK_API ARagnarokAIController : public AAIController
{
	GENERATED_BODY()

public:
	/** 생성자 */
	ARagnarokAIController(const FObjectInitializer& ObjectInitializer);

protected:
	/** Detour Crowd Avoidance 활성화 여부 멤버 변수 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Enable Detour Crowd Avoidance"))
	bool bEnableDetourCrowdAvoidance = true;

	/** Detour Crowd Avoidance 품질 (1: Low, 2: Medium, 3: Good, 4: High) */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bEnableDetourCrowdAvoidance", UIMin = "1", UIMax = "4", DisplayName = "Detour Crowd Quality"))
	int32 DetourCrowdAvoidanceQuality = 4;

	/** 군중 충돌 감지 범위 */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bEnableDetourCrowdAvoidance", DisplayName = "Crowd Collision Query Range"))
	float CrowdCollisionQueryRange = 600.0f;

};
	