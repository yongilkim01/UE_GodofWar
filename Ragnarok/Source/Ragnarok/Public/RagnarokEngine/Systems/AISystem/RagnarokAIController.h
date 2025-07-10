// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RagnarokAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class RAGNAROK_API ARagnarokAIController : public AAIController
{
	GENERATED_BODY()

public:
	ARagnarokAIController(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|AI")
	bool bEnableDetourCrowdAvoidance = true;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|AI", meta = (EditCondition = "bEnableDetourCrowdAvoidance", UMin = "1", UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|AI", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CrowdCollisionQueryRange = 600.0f;

};
	