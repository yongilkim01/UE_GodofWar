// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Camera/RagnarokSpringArmComponent.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"

#include "GameFramework/CharacterMovementComponent.h"

URagnarokSpringArmComponent::URagnarokSpringArmComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	DesiredCameraLagSpeed = DefaultCameraLagSpeed;
}

void URagnarokSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URagnarokSpringArmComponent::SetCameraMode(ERagnarokCameraMode NewCameraMode)
{
	CurrentCameraMode = NewCameraMode;

	switch (CurrentCameraMode)
	{
	case ERagnarokCameraMode::ERCM_None:
		DesiredTargetArmLength = DefaultArmLength;
		DesiredCameraLagSpeed = DefaultCameraLagSpeed;
		bEnableCameraLag = true;
		break;
	case ERagnarokCameraMode::EPCM_Running:
		DesiredTargetArmLength = RunningArmLength;
		DesiredCameraLagSpeed = RunningCameraLagSpeed;
		bEnableCameraLag = true;
		break;
	case ERagnarokCameraMode::EPCM_Combat:
		DesiredTargetArmLength = CombatArmLength;
		DesiredCameraLagSpeed = CombatCameraLagSpeed;
		bEnableCameraLag = true;
		break;
	}
}

void URagnarokSpringArmComponent::UpdateDynamicCameraLagSpeed(float DeltaTime)
{
	// OwnerCharacter의 이동 방향
	FVector OwnerMovementDirection = GetOwnerMovementDirection();
	float MovementSpeed = OwnerMovementDirection.Size();
	float CalcCameraLagSpeed = DefaultCameraLagSpeed;

	// 일정 속도 이하로 움직이고 있다면 기본 값 유지하고 아니라면 계산
	if (MovementSpeed >= MinMovementSpeedThreshold) // 50.0f
	{
		CalcCameraLagSpeed = CalculateDynamicCameraLagSpeed(OwnerMovementDirection);
	}

	CameraLagSpeed = FMath::FInterpTo(
		CameraLagSpeed,
		CalcCameraLagSpeed,
		DeltaTime,
		CameraLagInterpolateSpeed
	);
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

	// 월드 가중치에서 로컬 가중치로 변환
	// 캐릭터 기준 앞/뒤/옆을 판단하기 위한 Velocity
	FRotator CharacterRotation = OwnerCharacter->GetActorRotation();
	FVector LocalVelocity = CharacterRotation.UnrotateVector(Velocity);

	return LocalVelocity;
}

void URagnarokSpringArmComponent::InterpolateCameraSettings(float DeltaTime)
{
	TargetArmLength = FMath::FInterpTo(
		TargetArmLength,
		DesiredTargetArmLength,
		DeltaTime,
		5.0f
	);

	CameraLagSpeed = FMath::FInterpTo(
		CameraLagSpeed,
		DesiredCameraLagSpeed,
		DeltaTime,
		CameraLagIntepSpeed
	);
}

float URagnarokSpringArmComponent::CalculateDynamicCameraLagSpeed(const FVector& OwnerMovementDirection) const
{
	// OwnerCharacter 노말화
	FVector NormalDirection = OwnerMovementDirection.GetSafeNormal();

	// OwnerCharacter의 방향이 앞인지 뒤인지 판단하기 위한 변수
	float ForwardBackwardRatio = NormalDirection.X; // ~1.0f ~ 1.0f
	// OwnerCharacter의 옆 이동 여부를 판단하기 위한 변수
	float LateralRatio = FMath::Abs(NormalDirection.Y); // 0.0f ~ 1.0f
	
	// 앞/뒤 방향일 경우
	if (FMath::Abs(ForwardBackwardRatio) > LateralRatio)
	{
		// 앞 방향일 경우
		if (ForwardBackwardRatio > 0.0f)
		{
			return FMath::Lerp(DefaultCameraLagSpeed, ForwardCameraLagSpeed, ForwardBackwardRatio);
		}
		// 뒤 방향일 경우
		else
		{
			return FMath::Lerp(DefaultCameraLagSpeed, BackwardCameraLagSpeed, ForwardBackwardRatio);
		}
	}
	// 옆 방향일 경우
	else
	{
		return FMath::Lerp(DefaultCameraLagSpeed, LateralCameraLagSpeed, LateralRatio);
	}
}