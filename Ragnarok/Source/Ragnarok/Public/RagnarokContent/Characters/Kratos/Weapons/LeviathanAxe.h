// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "Components/TimelineComponent.h"
#include "LeviathanAxe.generated.h"

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

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	//~ Begin ARagnarokWeapon Interface.
	virtual void LoadWeaponDataAsset() override;
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject) override;
	//~ End ARagnarokWeapon Interface.

	virtual void ThrowWeapon(
		FRotator CameraRotation,
		FVector CameraLocation,
		FVector CameraForwardVector) override;

	virtual void RecallWeapon() override;

private:
	void SnapAxeLocationAndRotation(FRotator StartRotation, FVector SnapDirection, FVector SnapLocation);
	void RotateAxe();

protected:
	UPROPERTY(EditAnywhere)
	UTimelineComponent* WeaponRotTimelineComponent = nullptr;

	UPROPERTY(EditAnywhere)
	UTimelineComponent* WeaponThrowTraceTimelineComponent = nullptr;
	UPROPERTY(EditAnywhere)
	USceneComponent* PivotPointComponent = nullptr;
	UPROPERTY(EditAnywhere)
	USceneComponent* LodgePointComponent = nullptr;
	UPROPERTY(EditAnywhere)
	TMap<bool, USoundBase*> ThrowSoundMap;

private:
	UPROPERTY(EditAnywhere)
	UCurveFloat* WeaponRotationCurve = nullptr;
	FOnTimelineFloat WeaponRotTimelineTick;
	FOnTimelineEvent WeaponRotTimelineEnd;

	UPROPERTY(EditAnywhere)
	UCurveFloat* WeaponThrowTraceCurve = nullptr;
	FOnTimelineFloat WeaponThrowTraceTimelineTick;
	FOnTimelineEvent WeaponThrowTraceTimelineEnd;

	float ThrowDistance = 250.0f;
	float AxeSpinAxisOffset = 0.0f;
	float WeaponSpinRate = 1.0f;
	float ThrowSpeed = 2500.0f;

	bool bThrowSoundFlipFlop = true;
};
