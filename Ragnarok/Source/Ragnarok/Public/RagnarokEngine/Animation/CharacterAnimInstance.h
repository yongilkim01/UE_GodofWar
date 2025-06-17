// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Animation/RagnarokAnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class ARagnarokCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UCharacterAnimInstance : public URagnarokAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	ARagnarokCharacter* OwnerCharacter = nullptr;

	UPROPERTY()
	UCharacterMovementComponent* OwnerCharacterMovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationData|LocomotionData")
	float GroundSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationData|LocomotionData")
	bool bAcceleration = false;
};
