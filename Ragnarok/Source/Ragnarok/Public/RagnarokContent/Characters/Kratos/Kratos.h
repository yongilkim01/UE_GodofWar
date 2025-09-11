// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "GameplayTagContainer.h"
#include "Kratos.generated.h"

class AKratosController;
class AKratosWeapon;
class ARagnarokWeapon;
class USpringArmComponent;
class UCameraComponent;
class UInitDataAssetKratos;
class UCharacterPrimaryAssetKratos;
class UInputConfigDataAsset;
class UInputComponent;
class UKratosCombatComponent;
class UKratosUIComponent;
class UKratosGameplayAbility;
class UKratosControlComponent;
class UTimelineComponent;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class RAGNAROK_API AKratos : public ARagnarokCharacter
{
		GENERATED_BODY()
public:
	friend class UKratosGameplayAbility;
	friend class UKratosControlComponent;

public:
	AKratos();

	//~ Begin IUIInterface Interface.
	virtual URagnarokUIComponent* GetUIComponent() const override;
	//~ End IUIInterface Interface.

	void UnEquipWeapon(AKratosWeapon* KratosWeapon);
	void ZoomInCamera();
	void ZoomOutCamera();
	void ShakeCamera();

	bool IsRunning() const;
	bool IsAiming() const;
	FVector2D GetMovementInputVector() const;
	AKratosController* GetKratosController();
	ARagnarokWeapon* GetKratosWeapon();
	bool GetAimingTargetLocation(FVector InWeaponLocation, FVector& OutTargetLocation, float MaxRange = 5000.0f);

	void ThrowWeapon(AKratosWeapon* KratosWeapon);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//~ Begin ICombatInterface Interface.
	virtual UCombatComponent* GetCombatComponent() const override;
	//~ End ICombatInterface Interface.

private:
	UFUNCTION()
	void OnCameraZoomTimelineTick(float Value);
	UFUNCTION()
	void OnCameraZoomTimelineEnd();

	void LoadKratosDataAsset();
	void InitPrimaryData(UObject* PDAObject);

	void InputAbilityPressed(FGameplayTag InputTag);
	void InputAbilityReleased(FGameplayTag InputTag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Init")
	UInitDataAssetKratos* InitDA = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Input")
	UInputConfigDataAsset* InputConfigDA = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Ragnarok")
	UTimelineComponent* CameraZoomTimelineComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok")
	UCurveFloat* CameraZoomCurve = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* MainCameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Combat", meta = (AllowPrivateAccess = "true"))
	UKratosCombatComponent* KratosCombatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Combat", meta = (AllowPrivateAccess = "true"))
	UKratosUIComponent* KratosUIComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Combat", meta = (AllowPrivateAccess = "true"))
	UKratosControlComponent* KratosControlComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Kratos")
	FName UnequipWeaponSocket;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> ShakeCameraClass;

private:
	UCharacterPrimaryAssetKratos* CharacterPDA = nullptr;
	bool bRolling = false;
	bool bAttacking = false;
	float FixedCameraWorldZLocation = 0.0f;
	FTimerHandle RunningTimerHandle;
	TWeakObjectPtr<AKratosController> KratosController;

	float CameraZoomSpringArmLength = 100.0f;
	float IdleSpringArmLength = 0.0f;
	float CameraZoomTurnRate = 30.0f;
	float IdleCameraTurnRate = 50.0f;
	float CameraZoomMaxWalkSpeed = 250.0f;
	float IdleMaxWalkSpeed = 400.0f;
	float TimelineComponentPlayRate = 1.4f;

public:
	FORCEINLINE UKratosCombatComponent* GetKratosCombatComponent() const { return KratosCombatComponent; }
	FORCEINLINE UCameraComponent* GetKratosCameraComponent() const { return MainCameraComponent; }
	FORCEINLINE bool IsAttacking() const { return bAttacking; }
	FORCEINLINE bool IsRolling() const { return bRolling; }
};
