// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTDecorator.h"
#include "GameplayTagContainer.h"
#include "HasGameplayTagBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UHasGameplayTagBTDecorator : public URagnarokBTDecorator
{
	GENERATED_BODY()
	
public:
	UHasGameplayTagBTDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


protected:
	UPROPERTY(EditAnywhere, Category = "AI|FaceTarget")
	FBlackboardKeySelector ActorToCheckKey;

	UPROPERTY(EditAnywhere, Category = "AI|FaceTarget")
	FGameplayTag TagToCheck;

	UPROPERTY(EditAnywhere, Category = "AI|FaceTarget")
	bool bInverseConditionCheck = false;
};
