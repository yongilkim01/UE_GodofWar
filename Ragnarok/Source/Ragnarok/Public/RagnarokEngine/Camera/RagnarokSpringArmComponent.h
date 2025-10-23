// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokSpringArmComponent.generated.h"

class ARagnarokCharacter;

/**
 * 라그나로크 프로젝트용 SpringArmComponent 클래스
 */
UCLASS()
class RAGNAROK_API URagnarokSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	URagnarokSpringArmComponent();

protected:
	/** 카메라 보간 업데이트 하는 함수 */
	void InterpolateCameraSettings(float DeltaTime);
	/** 현재 카메라 모드를 변경하고 변경하려는 모드에 따라 SpringArm 설정을 변경하는 함수 */
	void SetCameraMode(ERagnarokCameraMode NewCameraMode);

	/** 기본 카메라 랙이 움직이는 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Default Camera Lag Speed", ClampMin = "0.1", ClampMax = "20.0"))
	float DefaultCameraLagSpeed = 10.0f;
	/** OwnerCharacter가 달릴 때 카메라 랙이 움직이는 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Running Camera Lag Speed", ClampMin = "0.1", ClampMax = "20.0"))
	float RunningCameraLagSpeed = 1.0f;
	/** OwnerCharacter가 전투할 때 카메라 랙이 움직이는 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Combat Camera Lag Speed", ClampMin = "0.1", ClampMax = "20.0"))
	float CombatCameraLagSpeed = 1.0f;

	/** 기본 카메라 암 길이 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Default Arm Length"))
	float DefaultArmLength = 170.0f;
	/** OwnerCharacter가 달릴 때 카메라 암 길이 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Running Arm Length"))
	float RunningArmLength = 220.0f;
	/** OwnerCharacter가 전투할 떄 카메라 암 길이 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Combat Arm Length"))
	float CombatArmLength = 220.0f;

	/** OwnerCharacter가 전투할 떄 카메라 암 길이 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Default Camera Lag Max Distance"))
	float DefaultCameraLagMaxDistance = 50.0f;
	/** OwnerCharacter가 전투할 떄 카메라 암 길이 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Running Camera Lag Max Distance"))
	float RunningCameraLagMaxDistance = 50.0f;
	/** OwnerCharacter가 전투할 떄 카메라 암 길이 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Combat Camera Lag Max Distance"))
	float CombatCameraLagMaxDistance = 50.0f;

	/** TargetArmLength가 보간되는 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Target Arm Length Interpolate Speed"))
	float TargetArmLengthInterpSpeed = 5.0f;
	/** CameraLagSpeed가 보간되는 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Camera Lag Speed Interpolate Speed"))
	float CameraLagSpeedInterpSpeed = 1.0f;

	ERagnarokCameraMode CurrentCameraMode = ERagnarokCameraMode::ERCM_None;
	float DesiredTargetArmLength = 170.0f;
	float DesiredCameraLagSpeed = 10.0f;
};
