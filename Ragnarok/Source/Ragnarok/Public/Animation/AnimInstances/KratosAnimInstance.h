// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstances/CharacterAnimInstance.h"
#include "KratosAnimInstance.generated.h"

class AKratos;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationData|References")
	AKratos* Kratos = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationData|LocomotionData")
	bool bRelax = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimationData|LocomotionData")
	float RelaxStateThreshold = 5.0f;

	float IdleElapsedTime;
};
