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
class URagnarokSpringArmComponent;
class UKratosSpringArmComponent;
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
 * 크레토스 플레이어 캐릭터 클래스
 * 갓 오브 워의 주인공으로, 전투, 이동, 무기 시스템을 담당하는 플레이어 캐릭터
 * 
 * Kratos player character class.
 * The protagonist of God of War, handling combat, movement, and weapon systems as the player character.
 */
UCLASS()
class RAGNAROK_API AKratos : public ARagnarokCharacter
{
	GENERATED_BODY()

	friend class UKratosGameplayAbility;
	friend class UKratosControlComponent;

public:
	/** 생성자 */
	AKratos();

	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	//~ End AActor Interface.

	//~ Begin IUIInterface Interface.
	virtual URagnarokUIComponent* GetUIComponent() const override;
	//~ End IUIInterface Interface.

	//~ Begin ICombatInterface Interface.
	virtual UCombatComponent* GetCombatComponent() const override;
	//~ End ICombatInterface Interface.

	/**
	 * 공격 시 무기를 공격 소켓에 장착하는 함수
	 *
	 * @param KratosWeapon 공격 소켓에 장착할 무기
	 */
	void EquipWeaponToAttackSocekt(AKratosWeapon* KratosWeapon);

	/**
	 * 공격 시 무기를 공격 소켓에서 해제하는 함수
	 *
	 * @param KratosWeapon 공격 소켓에서 해제할 무기
	 */
	void UnEquipWeaponToAttackSocekt(AKratosWeapon* KratosWeapon);

	/**
	 * 무기를 해제하는 함수
	 *
	 * @param KratosWeapon 해제할 무기
	 */
	void UnEquipWeapon(AKratosWeapon* KratosWeapon);

	/**
	 * 무기를 던지는 함수
	 *
	 * @param KratosWeapon 던질 무기
	 */
	void ThrowWeapon(AKratosWeapon* KratosWeapon);

	/** 카메라를 줌인하는 함수 */
	void ZoomInCamera();

	/** 카메라를 줌아웃하는 함수 */
	void ZoomOutCamera();

	/** 카메라를 흔드는 함수 */
	void ShakeCamera();

	/** 달리기 상태인지 확인하는 함수 */
	bool IsRunning() const;

	/** 조준 상태인지 확인하는 함수 */
	bool IsAiming() const;

	/** 이동 입력 벡터를 반환하는 함수 */
	FVector2D GetMovementInputVector() const;

	/** 크레토스 컨트롤러를 반환하는 함수 */
	AKratosController* GetKratosController();

	/** 크레토스 무기를 반환하는 함수 */
	ARagnarokWeapon* GetKratosWeapon();

	/**
	 * 조준 목표 위치를 구하는 함수
	 *
	 * @param InWeaponLocation 무기 위치
	 * @param OutTargetLocation 출력될 목표 위치
	 * @param MaxRange 최대 범위 (기본값: 5000.0f)
	 */
	bool GetAimingTargetLocation(FVector InWeaponLocation, FVector& OutTargetLocation, float MaxRange = 5000.0f);

	// Getters (Inline Functions)
	/** 크레토스 전투 컴포넌트를 반환하는 함수 */
	FORCEINLINE UKratosCombatComponent* GetKratosCombatComponent() const { return KratosCombatComponent; }

	/** 크레토스 카메라 컴포넌트를 반환하는 함수 */
	FORCEINLINE UCameraComponent* GetKratosCameraComponent() const { return MainCameraComponent; }

	/** 공격 중인지 확인하는 함수 */
	FORCEINLINE bool IsAttacking() const { return bAttacking; }

	/** 구르기 중인지 확인하는 함수 */
	FORCEINLINE bool IsRolling() const { return bRolling; }

protected:
	/** 초기화 데이터 에셋 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kratos|Data", meta = (DisplayName = "Init Data Asset"))
	UInitDataAssetKratos* InitDA = nullptr;

	/** 입력 설정 데이터 에셋 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kratos|Data", meta = (DisplayName = "Input Config Data Asset"))
	UInputConfigDataAsset* InputConfigDA = nullptr;

private:
	/**
	 * 카메라 줌 타임라인 틱 함수
	 *
	 * @param Value 타임라인 값
	 */
	UFUNCTION()
	void OnCameraZoomTimelineTick(float Value);

	/** 카메라 줌 타임라인 종료 함수 */
	UFUNCTION()
	void OnCameraZoomTimelineEnd();

	/** 크레토스 데이터 에셋을 로드하는 함수 */
	void LoadKratosDataAsset();

	/**
	 * 프라이머리 데이터를 초기화하는 함수
	 *
	 * @param PDAObject 프라이머리 데이터 에셋 오브젝트
	 */
	void InitPrimaryData(UObject* PDAObject);

	/**
	 * 입력 어빌리티 눌림 처리 함수
	 *
	 * @param InputTag 입력 태그
	 */
	void InputAbilityPressed(FGameplayTag InputTag);

	/**
	 * 입력 어빌리티 떼어짐 처리 함수
	 *
	 * @param InputTag 입력 태그
	 */
	void InputAbilityReleased(FGameplayTag InputTag);

	/** 카메라 줌 타임라인 컴포넌트 */
	UPROPERTY(EditAnywhere, Category = "Kratos|Components", meta = (DisplayName = "Camera Zoom Timeline Component"))
	UTimelineComponent* CameraZoomTimelineComponent = nullptr;

	/** 카메라 줌 커브 */
	UPROPERTY(EditAnywhere, Category = "Kratos|Camera", meta = (DisplayName = "Camera Zoom Curve"))
	UCurveFloat* CameraZoomCurve = nullptr;

	/** 메인 카메라 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kratos|Components", meta = (AllowPrivateAccess = "true", DisplayName = "Main Camera Component"))
	UCameraComponent* MainCameraComponent = nullptr;

	/** 스프링 암 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kratos|Components", meta = (AllowPrivateAccess = "true", DisplayName = "Spring Arm Component"))
	UKratosSpringArmComponent* SpringArmComponent = nullptr;

	/** 크레토스 전투 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kratos|Components", meta = (AllowPrivateAccess = "true", DisplayName = "Kratos Combat Component"))
	UKratosCombatComponent* KratosCombatComponent = nullptr;

	/** 크레토스 UI 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kratos|Components", meta = (AllowPrivateAccess = "true", DisplayName = "Kratos UI Component"))
	UKratosUIComponent* KratosUIComponent = nullptr;

	/** 크레토스 컨트롤 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kratos|Components", meta = (AllowPrivateAccess = "true", DisplayName = "Kratos Control Component"))
	UKratosControlComponent* KratosControlComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Kratos|Weapon", meta = (DisplayName = "Equip Weapon Socket"))
	FName EquipWeaponSocket;
	/** 무기 해제 소켓 이름 */
	UPROPERTY(EditAnywhere, Category = "Kratos|Weapon", meta = (DisplayName = "Unequip Weapon Socket"))
	FName UnequipWeaponSocket;
	UPROPERTY(EditAnywhere, Category = "Kratos|Weapon", meta = (DisplayName = "Equip Weapon Attack Socket"))
	FName EquipWeaponAttackSocket;
	/** 카메라 흔들기 클래스 */
	UPROPERTY(EditAnywhere, Category = "Kratos|Camera", meta = (DisplayName = "Shake Camera Class"))
	TSubclassOf<UCameraShakeBase> ShakeCameraClass;

	/** 캐릭터 프라이머리 데이터 에셋 */
	UCharacterPrimaryAssetKratos* CharacterPDA = nullptr;

	/** 구르기 상태 */
	bool bRolling = false;
	/** 공격 상태 */
	bool bAttacking = false;
	/** 고정 카메라 월드 Z 위치 */
	float FixedCameraWorldZLocation = 0.0f;
	/** 달리기 타이머 핸들 */
	FTimerHandle RunningTimerHandle;
	/** 크레토스 컨트롤러 약한 참조 */
	TWeakObjectPtr<AKratosController> KratosController;
	/** 카메라 줌 스프링 암 길이 */
	float CameraZoomSpringArmLength = 100.0f;
	/** 기본 스프링 암 길이 */
	float IdleSpringArmLength = 0.0f;
	/** 카메라 줌 회전 속도 */
	float CameraZoomTurnRate = 30.0f;
	/** 기본 카메라 회전 속도 */
	float IdleCameraTurnRate = 50.0f;
	/** 카메라 줌 최대 이동 속도 */
	float CameraZoomMaxWalkSpeed = 250.0f;
	/** 기본 최대 이동 속도 */
	float IdleMaxWalkSpeed = 400.0f;
	/** 타임라인 컴포넌트 재생 속도 */
	float TimelineComponentPlayRate = 1.4f;
};
