// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTService.h"
#include "OrientToTargetActorBTService.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UOrientToTargetActorBTService : public URagnarokBTService
{
	GENERATED_BODY()

	UOrientToTargetActorBTService();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector TargetActorKey;
	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationSpeed;
};
 