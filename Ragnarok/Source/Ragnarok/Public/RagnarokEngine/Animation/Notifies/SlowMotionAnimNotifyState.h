// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Animation/RagnarokAnimNotifyState.h"
#include "SlowMotionAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API USlowMotionAnimNotifyState : public URagnarokAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

protected:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|AnimNotify")
	float TimeDilation;
};
