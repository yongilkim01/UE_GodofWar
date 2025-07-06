// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageTakenEffectExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UDamageTakenEffectExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UDamageTakenEffectExecCalc();

	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
