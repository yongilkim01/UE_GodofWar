// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Animation/AnimNotifies/SlowMotionAnimNotifyState.h"

void USlowMotionAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp && MeshComp->GetWorld())
	{
		MeshComp->GetWorld()->GetWorldSettings()->SetTimeDilation(TimeDilation);
	}

}

void USlowMotionAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

void USlowMotionAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp && MeshComp->GetWorld())
	{
		MeshComp->GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	}
}
