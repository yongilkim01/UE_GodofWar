// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
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

protected:
	/** 캐릭터 이동 방향에 따라서 SocketOffset을 업데이트하는 함수. */
	void UpdateSocketOffset(float DeltaTime);
	/** OwnerCharacter의 방향을 반환하는 함수. */
	FVector GetOwnerMovementDirection() const;
	/** 이동 방향에 따라 Target Offset을 계산하는 함수. */
	float CalculateTargetOffset(const FVector& MoveDirection) const;

private:
	/** 앞으로 이동할 떄 최대 오프셋 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Forward Offset"))
	float MaxForwardOffset = 0.0f;
	/** 뒤로 이동할 떄 최대 오프셋 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Backward Offset"))
	float MaxBackwardOffset = -200.0f;
	/** 옆으로 이동할 떄 최대 오프셋 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Lateral Offset"))
	float MaxLateralOffset = -100.0f;
	/** 오프셋 보간 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Offset Interpolation Speed", ClampMin = "0.0"))
	float OffsetInterpolateSpeed = 5.0f;
	/** 오프셋 업데이트 기능 활성화 여부 멤버 변수 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Enable Update Offset"))
	bool bEnableUpdateOffset = true;
	/** 오프셋 초기화 이동 속도 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Min Movement Speed Threshold", ClampMin = "0.0"))
	float MinMovementSpeedThreshold = 50.0f;

	//UPROPERTY()
	//ARagnarokCharacter* OwnerCharacter = nullptr;
};
