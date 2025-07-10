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
};
	