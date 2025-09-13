// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "Components/TimelineComponent.h"
#include "LeviathanAxe.generated.h"

class UNiagaraComponent;
class USoundCue;

/**
 * 리바이어던 도끼 클래스
 * 크레토스의 주무기로, 던지기/회수/박힘 등의 기능을 가진 무기
 * 
 * Leviathan Axe class.
 * Kratos' primary weapon with throwing, recalling, and lodging capabilities.
 */
UCLASS()
class RAGNAROK_API ALeviathanAxe : public AKratosWeapon
{
	GENERATED_BODY()

public:
	/** 생성자 */
	ALeviathanAxe();

	/** 무기를 초기화하는 함수 */
	virtual void InitWeapon() override;

	//~ Begin AKratosWeapon Interface.
	
	/**
	 * 무기를 던지는 함수
	 * 카메라 방향으로 도끼를 던지며 회전과 추적 타임라인을 시작하는 함수
	 *
	 * @param CameraRotation 카메라 회전값
	 * @param CameraLocation 카메라 위치
	 * @param CameraForwardVector 카메라 전방 벡터 (던지기 방향)
	 */
	virtual void ThrowWeapon(FRotator CameraRotation, FVector CameraLocation, FVector CameraForwardVector) override;
	
	/** 무기를 회수하는 함수 */
	virtual void RecallWeapon() override;
	
	/** 무기를 정지시키는 함수 */
	virtual void StopWeapon() override;
	
	/** 무기 트레일을 시작하는 함수 */
	virtual void StartWeaponTrail() override;
	
	/** 무기 트레일을 종료하는 함수 */
	virtual void EndWeaponTrail() override;

	//~ End AKratosWeapon Interface.

protected:
	//~ Begin AActor Interface.
	
	/** 게임 시작 시 호출됨 */
	virtual void BeginPlay() override;
	
	/** 매 프레임 호출됨 */
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	//~ Begin ARagnarokWeapon Interface.
	
	/** 무기 데이터 에셋을 로드하는 함수 */
	virtual void LoadWeaponDataAsset() override;
	
	/**
	 * 무기 프라이머리 데이터 에셋을 비동기로 로드하는 함수
	 * 리바이어던 도끼의 메시와 관련 데이터를 비동기로 로드하는 함수
	 *
	 * @param PDAAssetObject 로드할 프라이머리 데이터 에셋 오브젝트
	 */
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject) override;
	//~ End ARagnarokWeapon Interface.

private:
	/** VFX 컴포넌트를 초기화하는 함수 */
	void InitVFX();
	
	/**
	 * 무기 던지기 시 도끼의 초기 위치와 회전을 카메라 기준으로 설정하는 함수
	 *
	 * @param StartRotation 도끼의 시작 회전값 (카메라 회전 + 축 오프셋)
	 * @param SnapDirection 카메라 위치 (실제로는 사용되지 않음)
	 * @param SnapLocation 카메라 전방 벡터 (던지기 방향)
	 */
	void SnapAxeLocationAndRotation(FRotator StartRotation, FVector SnapDirection, FVector SnapLocation);
	
	/** 도끼를 회전시키는 함수 */
	void RotateAxe();
	
	/** 충돌을 검사하는 함수 */
	void CheckHitCollision();
	
	/**
	 * 충돌 사운드를 재생하는 함수
	 *
	 * @param ImpactLocation 충돌 지점
	 */
	void PlayHitSoundCue(FVector ImpactLocation);
	
	/**
	 * 도끼를 표면에 박는 함수
	 *
	 * @param ImpactNormal 충돌 표면의 법선 벡터
	 * @param ImpactLocation 충돌 지점
	 */
	void LodgeAxe(FVector ImpactNormal, FVector ImpactLocation);
	
	/** 도끼를 정지시키는 함수 */
	void StopAxe();
	
	/**
	 * 도끼 충돌 각도를 계산하는 함수
	 *
	 * @param ImpactNormal 충돌 표면의 법선 벡터
	 * @param InclinedSurfaceRange 경사면 범위
	 * @param RegularSurfaceRange 일반 표면 범위
	 */
	float CalcAxeImpactPitch(FVector ImpactNormal, float InclinedSurfaceRange, float RegularSurfaceRange);
	
	/**
	 * 도끼 박힘 위치를 계산하는 함수
	 *
	 * @param ImpactNormal 충돌 표면의 법선 벡터
	 * @param ImpactLocation 충돌 지점
	 */
	FVector CalcAxeImactLocation(FVector ImpactNormal, FVector ImpactLocation);
	
	/** 박힌 도끼를 흔드는 함수 */
	void WiggleLodgedAxe();
	
	/**
	 * 소유자로부터 제한된 거리를 계산하는 함수
	 *
	 * @param MaxDistance 최대 거리
	 */
	float GetClampedDistanceFromOwnerCharacter(float MaxDistance);
	
	/** 무기 회수 위치를 설정하는 함수 */
	void SetWeaponRecallLocation();
	
	/**
	 * 회수 타임라인 재생 속도를 계산하는 함수
	 *
	 * @param Distance 거리
	 * @param WeaponRecallSpeed 무기 회수 속도
	 */
	float CalcRecallTimelinePlayRate(float Distance, float WeaponRecallSpeed);

	/** 지연 완료 시 호출되는 콜백 함수 */
	UFUNCTION()
	void OnDelayFinished();
	
	/**
	 * 무기 회전 타임라인 틱 시 호출되는 함수
	 *
	 * @param Value 타임라인 값
	 */
	UFUNCTION()
	void OnWeaponRotTimelineTick(float Value);
	
	/** 무기 회전 타임라인 종료 시 호출되는 함수 */
	UFUNCTION()
	void OnWeaponRotTimelineEnd();
	
	/**
	 * 무기 던지기 추적 타임라인 틱 시 호출되는 함수
	 *
	 * @param Value 타임라인 값
	 */
	UFUNCTION()
	void OnWeaponThrowTraceTimelineTick(float Value);
	
	/** 무기 던지기 추적 타임라인 종료 시 호출되는 함수 */
	UFUNCTION()
	void OnWeaponThrowTraceTimelineEnd();
	
	/**
	 * 무기 흔들기 타임라인 틱 시 호출되는 함수
	 *
	 * @param Value 타임라인 값
	 */
	UFUNCTION()
	void OnWeaponWiggleTimelineTick(float Value);
	
	/** 무기 흔들기 타임라인 종료 시 호출되는 함수 */
	UFUNCTION()
	void OnWeaponWiggleTimelineEnd();
	
	/**
	 * 무기 회수 타임라인 틱 시 호출되는 함수
	 *
	 * @param Value 타임라인 값
	 */
	UFUNCTION()
	void OnWeaponRecallTimelineTick(float Value);
	
	/** 무기 회수 타임라인 종료 시 호출되는 함수 */
	UFUNCTION()
	void OnWeaponRecallTimelineEnd();
	
	/**
	 * 무기 회수 회전 타임라인 틱 시 호출되는 함수
	 *
	 * @param Value 타임라인 값
	 */
	UFUNCTION()
	void OnWeaponRecallRotationTick(float Value);
	
	/** 무기 회수 회전 타임라인 종료 시 호출되는 함수 */
	UFUNCTION()
	void OnWeaponRecallRotationEnd();

protected:
	/** 도끼 회전의 중심점이 되는 컴포넌트 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Components", meta = (DisplayName = "Pivot Point"))
	USceneComponent* PivotPointComponent = nullptr;
	
	/** 도끼가 박힐 때의 기준점 컴포넌트 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Components", meta = (DisplayName = "Lodge Point"))
	USceneComponent* LodgePointComponent = nullptr;
	
	/** 도끼 날의 나이아가라 파티클 컴포넌트 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|VFX", meta = (DisplayName = "Blade Niagara"))
	UNiagaraComponent* BladeNiagaraComponent = nullptr;

	/** 던지기 사운드 맵 (true/false에 따른 다른 사운드) */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Audio", meta = (DisplayName = "Throw Sound Map"))
	TMap<bool, USoundBase*> ThrowSoundMap;
	
	/** 회수 사운드 맵 (true/false에 따른 다른 사운드) */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Audio", meta = (DisplayName = "Recall Sound Map"))
	TMap<bool, USoundBase*> RecallSoundMap;
	
	/** 첫 번째 충돌 사운드 큐 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeviathanAxe|Audio", meta = (DisplayName = "Hit Sound Cue 1"))
	USoundCue* HitSoundCue1 = nullptr;
	
	/** 두 번째 충돌 사운드 큐 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeviathanAxe|Audio", meta = (DisplayName = "Hit Sound Cue 2"))
	USoundCue* HitSoundCue2 = nullptr;
	
	/** 회수 노이즈 사운드 큐 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeviathanAxe|Audio", meta = (DisplayName = "Recall Noise Sound"))
	USoundCue* RecallNoiseSoundCue = nullptr;
	
	/** 회수 브라운 사운드 큐 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeviathanAxe|Audio", meta = (DisplayName = "Recall Brown Sound"))
	USoundCue* RecallBrownSoundCue = nullptr;
	
	/** 부착 사운드 감쇠 설정 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeviathanAxe|Audio", meta = (DisplayName = "Attach Sound Attenuation"))
	USoundAttenuation* AttachSoundAttenuation = nullptr;

private:
	/** 무기 회전 애니메이션 커브 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Animation Curves", meta = (DisplayName = "Weapon Rotation Curve"))
	UCurveFloat* WeaponRotationCurve = nullptr;
	
	/** 무기 던지기 추적 커브 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Animation Curves", meta = (DisplayName = "Weapon Throw Trace Curve"))
	UCurveFloat* WeaponThrowTraceCurve = nullptr;
	
	/** 무기 회수 흔들기 커브 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall", meta = (DisplayName = "Weapon Recall Wiggle Curve"))
	UCurveFloat* WeaponRecallWiggleCurve = nullptr;
	
	/** 회수 속도 커브 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall", meta = (DisplayName = "Recall Speed Curve"))
	UCurveFloat* RecallSpeedCurve = nullptr;
	
	/** 회수 사운드 볼륨 커브 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall", meta = (DisplayName = "Recall Sound Volume Curve"))
	UCurveFloat* RecallSoundVolCurve = nullptr;
	
	/** 회수 회전 커브 1 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall", meta = (DisplayName = "Recall Rotation Curve 1"))
	UCurveFloat* RecallRotationCurve1 = nullptr;
	
	/** 회수 회전 커브 2 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall", meta = (DisplayName = "Recall Rotation Curve 2"))
	UCurveFloat* RecallRotationCurve2 = nullptr;
	
	/** 회수 회전 커브 3 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall", meta = (DisplayName = "Recall Rotation Curve 3"))
	UCurveFloat* RecallRotationCurve3 = nullptr;
	
	/** 회수 오른쪽 벡터 커브 */
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall", meta = (DisplayName = "Recall Right Vector Curve"))
	UCurveFloat* RecallRightVectorCurve = nullptr;

	/** 회수 오디오 컴포넌트 */
	UAudioComponent* RecallAudioComponent = nullptr;
	
	/** 무기 회전 타임라인 컴포넌트 */
	UTimelineComponent* WeaponRotTimelineComponent = nullptr;
	
	/** 무기 던지기 추적 타임라인 컴포넌트 */
	UTimelineComponent* WeaponThrowTraceTimelineComponent = nullptr;
	
	/** 무기 흔들기 타임라인 컴포넌트 */
	UTimelineComponent* WeaponWiggleTimelineComponent = nullptr;
	
	/** 무기 회수 타임라인 컴포넌트 */
	UTimelineComponent* WeaponRecallTimelineComponent = nullptr;
	
	/** 무기 회수 회전 타임라인 컴포넌트 */
	UTimelineComponent* WeaponRecallRotationTimelineComponent = nullptr;

	/** 무기 회전 타임라인 틱 델리게이트 */
	FOnTimelineFloat WeaponRotTimelineTick;
	
	/** 무기 회전 타임라인 종료 델리게이트 */
	FOnTimelineEvent WeaponRotTimelineEnd;
	
	/** 무기 던지기 추적 타임라인 틱 델리게이트 */
	FOnTimelineFloat WeaponThrowTraceTimelineTick;
	
	/** 무기 던지기 추적 타임라인 종료 델리게이트 */
	FOnTimelineEvent WeaponThrowTraceTimelineEnd;
	
	/** 무기 흔들기 타임라인 틱 델리게이트 */
	FOnTimelineFloat WeaponWiggleTimelineTick;
	
	/** 무기 흔들기 타임라인 종료 델리게이트 */
	FOnTimelineEvent WeaponWiggleTimelineEnd;
	
	/** 무기 회수 타임라인 틱 델리게이트 */
	FOnTimelineFloat WeaponRecallTimelineTick;
	
	/** 무기 회수 타임라인 종료 델리게이트 */
	FOnTimelineEvent WeaponRecallTimelineEnd;
	
	/** 무기 회수 회전 타임라인 틱 델리게이트 */
	FOnTimelineFloat WeaponRecallRotationTick;
	
	/** 무기 회수 회전 타임라인 종료 델리게이트 */
	FOnTimelineEvent WeaponRecallRotationEnd;

	/** 지연 타이머 핸들 */
	FTimerHandle DelayTimerHandler;

	/** 피벗 초기 위치 */
	FVector PivotInitLocation = FVector::ZeroVector;
	
	/** 초기 위치 */
	FVector InitLocation = FVector::ZeroVector;
	
	/** 상단 위치 */
	FVector TopPosition = FVector::ZeroVector;
	
	/** 하단 위치 */
	FVector BottomPosition = FVector::ZeroVector;
	
	/** 회수 목표 위치 */
	FVector RecallTargetLocation = FVector::ZeroVector;
	
	/** 피벗 초기 회전 */
	FRotator PivotInitRotation = FRotator::ZeroRotator;
	
	/** 박힘 초기 회전 */
	FRotator LodgeInitRotation = FRotator::ZeroRotator;
	
	/** 초기 회전 */
	FRotator InitRotation = FRotator::ZeroRotator;
	
	/** 카메라 시작 회전 */
	FRotator CameraStartRotation = FRotator::ZeroRotator;
	
	/** 박힘 회전 */
	FRotator LodgeRotation = FRotator::ZeroRotator;

	/** 던지기 거리 */
	float ThrowDistance = 250.0f;
	
	/** 도끼 회전축 오프셋 */
	float AxeSpinAxisOffset = 0.0f;
	
	/** 무기 회전 속도 */
	float WeaponSpinRate = 1.0f;
	
	/** 던지기 속도 */
	float ThrowSpeed = 2500.0f;
	
	/** 던지기 플립플롭 시간 */
	float ThrowFlipFlopTime = 0.5f;
	
	/** 회수 플립플롭 시간 */
	float RecallFlipFlopTime = 0.5f;
	
	/** 던지기 추적 거리 */
	float ThrowTraceDistance = 60.0f;
	
	/** 계산된 Z 값 */
	float CalcZValue = 0.0f;
	
	/** 최대 계산 거리 */
	float MaxCalcDistance = 3000.0f;
	
	/** 소유자로부터의 거리 */
	float DistanceFromOwner = 0.0f;
	
	/** Z 조정값 */
	float ZAdjustment = 0.0f;
	
	/** 최적 거리 */
	float OptimalDistance = 1400.0f;
	
	/** 도끼 회수 속도 */
	float AxeRecallSpeed = 1.0f;
	
	/** 도끼 오른쪽 벡터 스케일 */
	float AxeRightVectorScale = 1.0f;
	
	/** 도끼 회수 회전값 */
	float AxeRecallRotationValue = 60.0f;
	
	/** 도끼 회수 회전 속도 */
	float AxeRecallSpinRate = 0.35f;
	
	/** 회수 타임라인 길이 */
	float LengthRecallTimeline = 0.0f;

	/** 회수 회전 횟수 */
	int32 RecallSpinCount = 0;

	/** 던지기 사운드 플립플롭 상태 */
	bool bThrowSoundFlipFlop = true;
	
	/** 회수 사운드 플립플롭 상태 */
	bool bRecallSoundFlipFlop = true;
};
