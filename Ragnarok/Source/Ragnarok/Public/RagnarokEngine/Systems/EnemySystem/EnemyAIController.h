// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokAIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API AEnemyAIController : public ARagnarokAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController(const FObjectInitializer& ObjectInitializer);

protected:
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ragnarok|AI")
	UAIPerceptionComponent* AIPerceptionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ragnarok|AI")
	UAISenseConfig_Sight* AISenseConfig_Sight = nullptr;
	
};
