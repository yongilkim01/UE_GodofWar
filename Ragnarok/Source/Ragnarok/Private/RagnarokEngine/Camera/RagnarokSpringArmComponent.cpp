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
	CameraLagMaxDistance = DefaultCameraLagMaxDistance;
	bEnableCameraLag = true;
}

void URagnarokSpringArmComponent::SetCameraMode(ERagnarokCameraMode NewCameraMode)
{
	CurrentCameraMode = NewCameraMode;

	switch (CurrentCameraMode)
	{
	case ERagnarokCameraMode::ERCM_None:
		DesiredTargetArmLength = DefaultArmLength;
		DesiredCameraLagSpeed = DefaultCameraLagSpeed;
		CameraLagMaxDistance = DefaultCameraLagMaxDistance;
		break;
	case ERagnarokCameraMode::EPCM_Running:
		DesiredTargetArmLength = RunningArmLength;
		DesiredCameraLagSpeed = RunningCameraLagSpeed;
		CameraLagMaxDistance = RunningCameraLagMaxDistance;
		break;
	case ERagnarokCameraMode::EPCM_Combat:
		DesiredTargetArmLength = CombatArmLength;
		DesiredCameraLagSpeed = CombatCameraLagSpeed;
		CameraLagMaxDistance = CombatCameraLagMaxDistance;
		break;
	}
}

void URagnarokSpringArmComponent::InterpolateCameraSettings(float DeltaTime)
{
	TargetArmLength = FMath::FInterpTo(
		TargetArmLength,
		DesiredTargetArmLength,
		DeltaTime,
		TargetArmLengthInterpSpeed
	);

	CameraLagSpeed = FMath::FInterpTo(
		CameraLagSpeed,
		DesiredCameraLagSpeed,
		DeltaTime,
		CameraLagSpeedInterpSpeed
	);
}