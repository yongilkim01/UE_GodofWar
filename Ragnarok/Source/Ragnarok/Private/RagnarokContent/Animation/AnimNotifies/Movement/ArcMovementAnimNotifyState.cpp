// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Animation/AnimNotifies/Movement/ArcMovementAnimNotifyState.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"

UArcMovementAnimNotifyState::UArcMovementAnimNotifyState()
{
}

void UArcMovementAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	RagnarokCharacter = Cast<ARagnarokCharacter>(MeshComp->GetOwner());

	if (nullptr == RagnarokCharacter) return;

	StartLocation = RagnarokCharacter->GetActorLocation();
	ForwardDirection = bMoveWorldSpace ? FVector::ForwardVector : RagnarokCharacter->GetActorForwardVector();
	TargetLocation = StartLocation + (ForwardDirection * MovementDistance);
	ElapsedTime = 0.0f;

	if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
	{
		if (UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage())
		{
			float PlayRate = AnimInstance->Montage_GetPlayRate(CurrentMontage);

			if (PlayRate > 0.0f) Duration = TotalDuration / PlayRate;
		}
	}
}

void UArcMovementAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (nullptr == RagnarokCharacter) return;

	ElapsedTime += FrameDeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

	if (nullptr != ArcMovementCurve)
	{
		Alpha = ArcMovementCurve->GetFloatValue(Alpha);
	}

	FVector CurrentLocation = CalcArcLocation(Alpha);

	FHitResult HitResult;
	RagnarokCharacter->SetActorLocation(CurrentLocation, true, &HitResult);
}

void UArcMovementAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (nullptr != RagnarokCharacter)
	{
		FHitResult HitResult;
		RagnarokCharacter->SetActorLocation(TargetLocation, true, &HitResult);
	}

	RagnarokCharacter = nullptr;

}

FVector UArcMovementAnimNotifyState::CalcArcLocation(float Alpha) const
{
	switch (ArcMovementType)
	{
	case EArcMovementType::EAMT_Jump:
	case EArcMovementType::EAMT_Leap:
	case EArcMovementType::EAMT_Parabolic:
		return CalcParablicLocation(Alpha);
	case EArcMovementType::EAMT_Custom:
		if (CustomArcPointArray.Num() >= 2)
		{
			
		}
		return CalcParablicLocation(Alpha);
	default:
		return CalcParablicLocation(Alpha);
	}
}

FVector UArcMovementAnimNotifyState::CalcParablicLocation(float Alpha) const
{
	FVector HorizontalLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);

	float HeightMul = 4.0f * Alpha * (1.0f - Alpha);
	float CurrentHeight = ArcHeight * HeightMul;

	return HorizontalLocation + FVector(0.0f, 0.0f, CurrentHeight);
}
