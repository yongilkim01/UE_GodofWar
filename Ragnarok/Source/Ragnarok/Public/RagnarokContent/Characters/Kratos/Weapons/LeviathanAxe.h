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
		FRotator CameraRotator,
		FVector ThrowDirection,
		FVector CameraLocation) override;

	virtual void ThrowWeaponToTarget(
		FVector StartLocation,
		FVector TargetLocation) override;

private:
	void SnapAxeLocationAndRotation(FRotator StartRotation, FVector SnapDirection, FVector SnapLocation);
	void RotateAxe();

private:
	UPROPERTY(EditAnywhere)
	UTimelineComponent* WeaponRotTimelineComponent = nullptr;

	UPROPERTY(EditAnywhere)
	UCurveFloat* WeaponRotationCurve = nullptr;

	FOnTimelineFloat WeaponRotTimelineTick;
	FOnTimelineEvent WeaponRotTimelineEnd;
	float ThrowDistance = 250.0f;
	float AxeSpinAxisOffset = 100.0f;
	float WeaponSpinRate = 1.0f;
	float ThrowSpeed = 30000.0f;
};
