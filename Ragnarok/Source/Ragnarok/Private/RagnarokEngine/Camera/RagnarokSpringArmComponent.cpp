// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Camera/RagnarokSpringArmComponent.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

URagnarokSpringArmComponent::URagnarokSpringArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void URagnarokSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (true == bEnableUpdateOffset)
	{
		UpdateSocketOffset(DeltaTime);
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URagnarokSpringArmComponent::UpdateSocketOffset(float DeltaTime)
{
	FVector OwnerMovementDirection = GetOwnerMovementDirection();

	float MovememtSpeed = OwnerMovementDirection.Size();
	float CurrentOffsetX = SocketOffset.X;
	if (MovememtSpeed < MinMovementSpeedThreshold)
	{
		SocketOffset.X = FMath::FInterpTo(CurrentOffsetX, 0.0f, DeltaTime, OffsetInterpolateSpeed);
	}
	else
	{
		float TargetOffsetX = CalculateTargetOffset(OwnerMovementDirection);
		SocketOffset.X = FMath::FInterpTo(CurrentOffsetX, TargetOffsetX, DeltaTime, OffsetInterpolateSpeed);
	}
}

FVector URagnarokSpringArmComponent::GetOwnerMovementDirection() const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (nullptr == OwnerCharacter || nullptr == OwnerCharacter->GetCharacterMovement())
	{
		return FVector::ZeroVector;
	}

	FVector Velocity = OwnerCharacter->GetCharacterMovement()->Velocity;

	if (true == Velocity.IsNearlyZero())
	{
		return FVector::ZeroVector;
	}

	// 월드 좌표에서 로컬 좌표로 변환
	FRotator CharacterRotation = OwnerCharacter->GetActorRotation();
	FVector LocalVelocity = CharacterRotation.UnrotateVector(Velocity);

	return LocalVelocity;
}

float URagnarokSpringArmComponent::CalculateTargetOffset(const FVector& MovementDirection) const
{
	float DirectionX = MovementDirection.X;
	float DirectionY = MovementDirection.Y;
	FVector NormalDirection = MovementDirection.GetSafeNormal();
	float ForwardBackwardRatio = NormalDirection.X;
	float LateralRatio = FMath::Abs(NormalDirection.Y);
	float CalcTargetOffset = 0.0f;
	
	if (ForwardBackwardRatio < 0.0f)
	{
		CalcTargetOffset = FMath::Lerp(0.0f, MaxBackwardOffset, FMath::Abs(ForwardBackwardRatio));
	}
	else if (ForwardBackwardRatio > 0.0f)
	{
		CalcTargetOffset = FMath::Lerp(0.0f, MaxForwardOffset, ForwardBackwardRatio);
	}

	if (LateralRatio > 0.5f)
	{
		float LateralOffset = FMath::Lerp(0.0f, MaxLateralOffset, LateralRatio);
		CalcTargetOffset = FMath::Lerp(CalcTargetOffset, LateralOffset, 0.5f);
	}

	return CalcTargetOffset;
}