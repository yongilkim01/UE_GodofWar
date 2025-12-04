// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Animation/AnimNotifies/AnimPlayRateNotifyState.h"

void UAnimPlayRateNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	AnimInstance = MeshComp->GetAnimInstance();
	
	if(nullptr != AnimInstance)
	{
		CurAnimMontage = AnimInstance->GetCurrentActiveMontage();
		
		if(nullptr != CurAnimMontage)
		{
			OldPlayRate = AnimInstance->Montage_GetPlayRate(CurAnimMontage);
			AnimInstance->Montage_SetPlayRate(CurAnimMontage, NewPlayRate);
		}
	}

}

void UAnimPlayRateNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

void UAnimPlayRateNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (nullptr == AnimInstance || nullptr == CurAnimMontage) return;

	AnimInstance->Montage_SetPlayRate(CurAnimMontage, OldPlayRate);
}
