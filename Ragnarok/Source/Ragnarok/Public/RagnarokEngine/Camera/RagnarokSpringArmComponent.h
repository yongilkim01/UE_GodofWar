// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokSpringArmComponent.generated.h"

class ARagnarokCharacter;

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	URagnarokSpringArmComponent();

	//~ Begin UActorComponent Interface.
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface.

	void SetCameraMode(ERagnarokCameraMode NewCameraMode);

protected:
	/** 캐릭터 이동 방향에 따라서 SocketOffset을 업데이트하는 함수. */
	void UpdateDynamicCameraLagSpeed(float DeltaTime);
	/** 이동 방향에 따라 Target Offset을 계산하는 함수. */
	float CalculateDynamicCameraLagSpeed(const FVector& OwnerMovementDirection) const;
	/** OwnerCharacter의 방향을 반환하는 함수. */
	FVector GetOwnerMovementDirection() const;
	/** 카메라 보간 업데이트 하는 함수 */
	void InterpolateCameraSettings(float DeltaTime);

	/** 카메라 래그 보간 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Camera Lag Interpolate Speed", ClampMin = "0.0"))
	float CameraLagInterpolateSpeed = 5.0f;
	/**  업데이트 최소 임계값 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Min Movement Speed Threshold", ClampMin = "0.0"))
	float MinMovementSpeedThreshold = 50.0f;
	/** 카메라 래그 활성화 여부 멤버 변수 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Enable Dynamic Camera Lag"))
	bool bEnableDynamicCameraLag = true;\
	/** 기본 카메라 랙이 움직이는 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Default Camera Lag Speed"))
	float DefaultCameraLagSpeed = 3.0f;
	/** 앞으로 움직일 떄 카메라 랙이 움직이는 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Forward Camera Lag Speed"))
	float ForwardCameraLagSpeed = 3.0f;
	/** 뒤로 움직일 떄 카메라 랙이 움직이는 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Backward Camera Lag Speed"))
	float BackwardCameraLagSpeed = 15.0f;
	/** 옆으로 움직일 떄 카메라 랙이 움직이는 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Lateral Camera Lag Speed"))
	float LateralCameraLagSpeed = 12.0f;
	/** 기본 카메라 암 길이 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Default Arm Length"))
	float DefaultArmLength = 300.0f;
	/** OwnerCharacter가 달릴 때 카메라 암 길이 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Running Arm Length"))
	float RunningArmLength = 400.0f;
	/** OwnerCharacter가 전투할 떄 카메라 암 길이 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Combat Arm Length"))
	float CombatArmLength = 350.0f;

	ERagnarokCameraMode CurrentCameraMode = ERagnarokCameraMode::ERCM_None;
	float DesiredTargetArmLength = 300.0f;
};
