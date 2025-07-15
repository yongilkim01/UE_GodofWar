// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTDecorator.h"
#include "ComputeSucessChanceBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UComputeSucessChanceBTDecorator : public URagnarokBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI|Decorator", meta = (UIMin = "0.0", UIMax = "1.0", SliderExponent = "1.0"))
	float SuccessChanceMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "AI|Decorator", meta = (UIMin = "0.0", UIMax = "1.0", SliderExponent = "1.0"))
	float SuccessChanceMax = 0.5f;
};
