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

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	//~ Begin ARagnarokWeapon Interface.
	virtual void LoadWeaponDataAsset() override;
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject) override;
	//~ End ARagnarokWeapon Interface.

	//~ Begin AKratosWeapon Interface.
	virtual void ThrowWeapon(FRotator CameraRotation, FVector CameraLocation, FVector CameraForwardVector) override;
	virtual void RecallWeapon() override;
	virtual void StopWeapon() override;
	//~ End AKratosWeapon Interface.

private:
	void InitVFX();
	void SnapAxeLocationAndRotation(FRotator StartRotation, FVector SnapDirection, FVector SnapLocation);
	void RotateAxe();
	void StartWeaponTrail();
	void EndWeaponTrail();
	void CheckHitCollision();
	void PlayHitSoundCue(FVector ImpactLocation);
	void LodgeAxe(FVector ImpactNormal, FVector ImpactLocation);
	void StopAxe();
	float CalcAxeImpactPitch(FVector ImpactNormal, float InclinedSurfaceRange, float RegularSurfaceRange);
	FVector CalcAxeImactLocation(FVector ImpactNormal, FVector ImpactLocation);
	void WiggleLodgedAxe();
	float GetClampedDistanceFromOwnerCharacter(float MaxDistance);

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
	USoundCue* HitSoundCue1 = nullptr;

	UPROPERTY(EditAnywhere)
	USoundCue* HitSoundCue2 = nullptr;

	UPROPERTY(EditAnywhere)
	USoundCue* RecallNoiseSoundCue = nullptr;

	UPROPERTY(EditAnywhere)
	USoundAttenuation* AttachSoundAttenuation = nullptr;

private:
	UPROPERTY(EditAnywhere)
	UCurveFloat* WeaponRotationCurve = nullptr;
	UPROPERTY(EditAnywhere)
	UCurveFloat* WeaponThrowTraceCurve = nullptr;
	UPROPERTY(EditAnywhere)
	UCurveFloat* WeaponWiggleCurve = nullptr;

	UAudioComponent* RecallAudioComponent = nullptr;
	UTimelineComponent* WeaponRotTimelineComponent = nullptr;
	UTimelineComponent* WeaponThrowTraceTimelineComponent = nullptr;
	UTimelineComponent* WeaponWiggleTimelineComponent = nullptr;

	FOnTimelineFloat WeaponRotTimelineTick;
	FOnTimelineEvent WeaponRotTimelineEnd;
	FOnTimelineFloat WeaponThrowTraceTimelineTick;
	FOnTimelineEvent WeaponThrowTraceTimelineEnd;
	FOnTimelineFloat WeaponWiggleTimelineTick;
	FOnTimelineEvent WeaponWiggleTimelineEnd;

	FVector TopPosition = FVector::ZeroVector;
	FVector BottomPosition = FVector::ZeroVector;
	FRotator CameraStartRotation = FRotator::ZeroRotator;
	FRotator LodgeRotation = FRotator::ZeroRotator;

	float ThrowDistance = 250.0f;
	float AxeSpinAxisOffset = 0.0f;
	float WeaponSpinRate = 1.0f;
	float ThrowSpeed = 2500.0f;
	float ThrowFlipFlopTime = 0.5f;
	float ThrowTraceDistance = 60.0f;
	float CalcZValue = 0.0f;
	float MaxCalcDistance = 3000.0f;
	float DistanceFromOwner = 0.0f;

	bool bThrowSoundFlipFlop = true;
};
