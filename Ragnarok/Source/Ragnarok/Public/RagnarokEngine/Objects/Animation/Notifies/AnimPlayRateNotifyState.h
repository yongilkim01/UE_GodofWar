// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/Animation/RagnarokAnimNotifyState.h"
#include "AnimPlayRateNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UAnimPlayRateNotifyState : public URagnarokAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

private:
	UPROPERTY(EditAnywhere)
	float NewPlayRate = 0.0f;
	UPROPERTY()
	UAnimInstance* AnimInstance = nullptr;
	UPROPERTY()
	UAnimMontage* CurAnimMontage = nullptr;

	float OldPlayRate = 0.0f;

};
