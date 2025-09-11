// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "Components/TimelineComponent.h"
#include "LeviathanAxe.generated.h"

class UNiagaraComponent;
class USoundCue;

/**
 * 
 */
UCLASS()
class RAGNAROK_API ALeviathanAxe : public AKratosWeapon
{
	GENERATED_BODY()

public:
	ALeviathanAxe();

	virtual void InitWeapon() override;

	//~ Begin AKratosWeapon Interface.
	virtual void ThrowWeapon(FRotator CameraRotation, FVector CameraLocation, FVector CameraForwardVector) override;
	virtual void RecallWeapon() override;
	virtual void StopWeapon() override;
	virtual void StartWeaponTrail() override;
	virtual void EndWeaponTrail() override;
	//~ Begin AKratosWeapon Interface.

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	//~ Begin ARagnarokWeapon Interface.
	virtual void LoadWeaponDataAsset() override;
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject) override;
	//~ End ARagnarokWeapon Interface.

private:
	void InitVFX();
	void SnapAxeLocationAndRotation(FRotator StartRotation, FVector SnapDirection, FVector SnapLocation);
	void RotateAxe();
	void CheckHitCollision();
	void PlayHitSoundCue(FVector ImpactLocation);
	void LodgeAxe(FVector ImpactNormal, FVector ImpactLocation);
	void StopAxe();
	float CalcAxeImpactPitch(FVector ImpactNormal, float InclinedSurfaceRange, float RegularSurfaceRange);
	FVector CalcAxeImactLocation(FVector ImpactNormal, FVector ImpactLocation);
	void WiggleLodgedAxe();	
	float GetClampedDistanceFromOwnerCharacter(float MaxDistance);
	void SetWeaponRecallLocation();
	float CalcRecallTimelinePlayRate(float Distance, float WeaponRecallSpeed);

	UFUNCTION()
	void OnDelayFinished();
	UFUNCTION()
	void OnWeaponRotTimelineTick(float Value);
	UFUNCTION()
	void OnWeaponRotTimelineEnd();
	UFUNCTION()
	void OnWeaponThrowTraceTimelineTick(float Value);
	UFUNCTION()
	void OnWeaponThrowTraceTimelineEnd();
	UFUNCTION()
	void OnWeaponWiggleTimelineTick(float Value);
	UFUNCTION()
	void OnWeaponWiggleTimelineEnd();
	UFUNCTION()
	void OnWeaponRecallTimelineTick(float Value);
	UFUNCTION()
	void OnWeaponRecallTimelineEnd();
	UFUNCTION()
	void OnWeaponRecallRotationTick(float Value);
	UFUNCTION()
	void OnWeaponRecallRotationEnd();


protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* PivotPointComponent = nullptr;
	UPROPERTY(EditAnywhere)
	USceneComponent* LodgePointComponent = nullptr;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* BladeNiagaraComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TMap<bool, USoundBase*> ThrowSoundMap;
	UPROPERTY(EditAnywhere)
	TMap<bool, USoundBase*> RecallSoundMap;
	UPROPERTY(EditAnywhere)
	USoundCue* HitSoundCue1 = nullptr;
	UPROPERTY(EditAnywhere)
	USoundCue* HitSoundCue2 = nullptr;
	UPROPERTY(EditAnywhere)
	USoundCue* RecallNoiseSoundCue = nullptr;
	UPROPERTY(EditAnywhere)
	USoundCue* RecallBrownSoundCue = nullptr;
	UPROPERTY(EditAnywhere)
	USoundAttenuation* AttachSoundAttenuation = nullptr;

private:
	UPROPERTY(EditAnywhere)
	UCurveFloat* WeaponRotationCurve = nullptr;
	UPROPERTY(EditAnywhere)
	UCurveFloat* WeaponThrowTraceCurve = nullptr;
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall")
	UCurveFloat* WeaponRecallWiggleCurve = nullptr;
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall")
	UCurveFloat* RecallSpeedCurve = nullptr;
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall")
	UCurveFloat* RecallSoundVolCurve = nullptr;
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall")
	UCurveFloat* RecallRotationCurve1 = nullptr;
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall")
	UCurveFloat* RecallRotationCurve2 = nullptr;
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall")
	UCurveFloat* RecallRotationCurve3 = nullptr;
	UPROPERTY(EditAnywhere, Category = "LeviathanAxe|Recall")
	UCurveFloat* RecallRightVectorCurve = nullptr;

	UAudioComponent* RecallAudioComponent = nullptr;
	UTimelineComponent* WeaponRotTimelineComponent = nullptr;
	UTimelineComponent* WeaponThrowTraceTimelineComponent = nullptr;
	UTimelineComponent* WeaponWiggleTimelineComponent = nullptr;
	UTimelineComponent* WeaponRecallTimelineComponent = nullptr;
	UTimelineComponent* WeaponRecallRotationTimelineComponent = nullptr;

	FOnTimelineFloat WeaponRotTimelineTick;
	FOnTimelineEvent WeaponRotTimelineEnd;
	FOnTimelineFloat WeaponThrowTraceTimelineTick;
	FOnTimelineEvent WeaponThrowTraceTimelineEnd;
	FOnTimelineFloat WeaponWiggleTimelineTick;
	FOnTimelineEvent WeaponWiggleTimelineEnd;
	FOnTimelineFloat WeaponRecallTimelineTick;
	FOnTimelineEvent WeaponRecallTimelineEnd;
	FOnTimelineFloat WeaponRecallRotationTick;
	FOnTimelineEvent WeaponRecallRotationEnd;

	FTimerHandle DelayTimerHandler;

	FVector PivotInitLocation = FVector::ZeroVector;
	FVector InitLocation = FVector::ZeroVector;
	FVector TopPosition = FVector::ZeroVector;
	FVector BottomPosition = FVector::ZeroVector;
	FVector RecallTargetLocation = FVector::ZeroVector;
	FRotator PivotInitRotation = FRotator::ZeroRotator;
	FRotator LodgeInitRotation = FRotator::ZeroRotator;
	FRotator InitRotation = FRotator::ZeroRotator;
	FRotator CameraStartRotation = FRotator::ZeroRotator;
	FRotator LodgeRotation = FRotator::ZeroRotator;

	float ThrowDistance = 250.0f;
	float AxeSpinAxisOffset = 0.0f;
	float WeaponSpinRate = 1.0f;
	float ThrowSpeed = 2500.0f;
	float ThrowFlipFlopTime = 0.5f;
	float RecallFlipFlopTime = 0.5f;
	float ThrowTraceDistance = 60.0f;
	float CalcZValue = 0.0f;
	float MaxCalcDistance = 3000.0f;
	float DistanceFromOwner = 0.0f;
	float ZAdjustment = 0.0f;
	float OptimalDistance = 1400.0f;
	float AxeRecallSpeed = 1.0f;
	float AxeRightVectorScale = 1.0f;
	float AxeRecallRotationValue = 60.0f;
	float AxeRecallSpinRate = 0.35f;
	float LengthRecallTimeline = 0.0f;

	int32 RecallSpinCount = 0;

	bool bThrowSoundFlipFlop = true;
	bool bRecallSoundFlipFlop = true;
};
