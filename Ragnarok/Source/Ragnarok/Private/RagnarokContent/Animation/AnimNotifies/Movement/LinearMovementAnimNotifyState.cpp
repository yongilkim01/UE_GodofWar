// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Animation/AnimNotifies/Movement/LinearMovementAnimNotifyState.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"

ULinearMovementAnimNotifyState::ULinearMovementAnimNotifyState()
{
}

void ULinearMovementAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	RagnarokCharacter = Cast<ARagnarokCharacter>(MeshComp->GetOwner());

	if (nullptr == RagnarokCharacter) return;

	StartLocation = RagnarokCharacter->GetActorLocation();
	ElapsedTime = 0.0f;
	Duration = TotalDuration;

	FVector Direction = GetDirectionVector();
	TargetLocation = StartLocation + (Direction * MovementDistance);
}

void ULinearMovementAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (nullptr == RagnarokCharacter) return;

	ElapsedTime += FrameDeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

	if (nullptr != MovementSpeedCurve)
	{
		Alpha = MovementSpeedCurve->GetFloatValue(Alpha);
	}

	FVector CurrentLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);

	FHitResult HitResult;
	RagnarokCharacter->SetActorLocation(CurrentLocation, true, &HitResult);
}

void ULinearMovementAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (nullptr != RagnarokCharacter)
	{
		FHitResult HitResult;
		RagnarokCharacter->SetActorLocation(TargetLocation, true, &HitResult);
	}

	RagnarokCharacter = nullptr;
}

FVector ULinearMovementAnimNotifyState::GetDirectionVector() const
{
	if (nullptr == RagnarokCharacter) return FVector::ForwardVector;

	FVector Direction = FVector::ZeroVector;

	switch (MovementDirection)
	{
	case ERagnarokDirection::ERD_Forward:
		Direction = bMoveWorldSpace ? FVector::ForwardVector : RagnarokCharacter->GetActorForwardVector();
		break;
	case ERagnarokDirection::ERD_Backward:
		Direction = bMoveWorldSpace ? FVector::ForwardVector : -RagnarokCharacter->GetActorForwardVector();
		break;
	case ERagnarokDirection::ERD_Right:
		Direction = bMoveWorldSpace ? FVector::ForwardVector : RagnarokCharacter->GetActorRightVector();
		break;
	case ERagnarokDirection::ERD_Left:
		Direction = bMoveWorldSpace ? FVector::ForwardVector : -RagnarokCharacter->GetActorRightVector();
		break;
	case ERagnarokDirection::ERD_ForwardRight:
	{
		FVector ForwardVector = bMoveWorldSpace ? FVector::ForwardVector : RagnarokCharacter->GetActorForwardVector();
		FVector RightVector = bMoveWorldSpace ? FVector::RightVector : RagnarokCharacter->GetActorRightVector();
		Direction = (ForwardVector + RightVector).GetSafeNormal();
	}
	break;
	case ERagnarokDirection::ERD_ForwardLeft:
	{
		FVector ForwardVector = bMoveWorldSpace ? FVector::ForwardVector : RagnarokCharacter->GetActorForwardVector();
		FVector LeftVector = bMoveWorldSpace ? FVector::LeftVector : -RagnarokCharacter->GetActorRightVector();
		Direction = (ForwardVector + LeftVector).GetSafeNormal();
	}
	break;
	case ERagnarokDirection::ERD_BackwardRight:
	{
		FVector BackwardVector = bMoveWorldSpace ? FVector::BackwardVector : -RagnarokCharacter->GetActorForwardVector();
		FVector RightVector = bMoveWorldSpace ? FVector::RightVector : RagnarokCharacter->GetActorRightVector();
		Direction = (BackwardVector + RightVector).GetSafeNormal();
	}
	break;
	case ERagnarokDirection::ERD_BackwardLeft:
	{
		FVector BackwardVector = bMoveWorldSpace ? FVector::BackwardVector : -RagnarokCharacter->GetActorForwardVector();
		FVector LeftVector = bMoveWorldSpace ? FVector::LeftVector : -RagnarokCharacter->GetActorRightVector();
		Direction = (BackwardVector + LeftVector).GetSafeNormal();
	}
	break;
	case ERagnarokDirection::ERD_Custom:
		Direction = bMoveWorldSpace ? 
			CustomDirection.GetSafeNormal() :
			RagnarokCharacter->GetActorTransform().TransformVectorNoScale(CustomDirection.GetSafeNormal());
		break;
	default:
		Direction = RagnarokCharacter->GetActorForwardVector();
		break;
	}

	return Direction;
}
