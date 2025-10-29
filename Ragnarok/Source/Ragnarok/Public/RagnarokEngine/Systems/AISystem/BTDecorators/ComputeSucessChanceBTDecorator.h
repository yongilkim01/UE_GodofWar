// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTDecorator.h"
#include "ComputeSucessChanceBTDecorator.generated.h"

/**
 * 설정된 확률 범위에 따라 랜덤 값을 계산해서 실행 여부를 결정하는 BTDecorator 클래스
 */
UCLASS()
class RAGNAROK_API UComputeSucessChanceBTDecorator : public URagnarokBTDecorator
{
	GENERATED_BODY()

protected:
	/** SuccessChanceMin/Max 사이의 랜덤 확률을 생성하고, 0~1 사이의 랜덤 값과 비교하여 성공 여부를 결정하는 함수 */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	/** 성공 확률 최솟값 (0.0 ~ 1.0) */
	UPROPERTY(EditAnywhere, Category = "ComputeSuccessChance|Probability", meta = (DisplayName = "Success Chance Min", UIMin = "0.0", UIMax = "1.0", SliderExponent = "1.0"))
	float SuccessChanceMin = 0.5f;

	/** 성공 확률 최댓값 (0.0 ~ 1.0) */
	UPROPERTY(EditAnywhere, Category = "ComputeSuccessChance|Probability", meta = (DisplayName = "Success Chance Max", UIMin = "0.0", UIMax = "1.0", SliderExponent = "1.0"))
	float SuccessChanceMax = 0.5f;
};
