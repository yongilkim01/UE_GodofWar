// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
#include "GameplayTagContainer.h"
#include "KratosLightAttackAbility.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;
class URKAbilityTask_RotateToCamera;
class AKratosWeapon;

/**
 * 크레토스 약공격 어빌리티 클래스
 */
UCLASS()
class RAGNAROK_API UKratosLightAttackAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()

public:
	/** 생성자 */
	UKratosLightAttackAbility();

	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~ End UGameplayAbility Interface.

	//~ Begin UKratosGameplayAbility Interface.
	virtual void OnMontageCompleted() override;
	virtual void OnMontageBlendOut() override;
	virtual void OnMontageInterrupted() override;
	//~ End UKratosGameplayAbility Interface.

protected:
	/**
	 * 데미지 처리를 처리하는 콜백 함수
	 */
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
	/**
	 * 콤보 입력 대기 시작 처리하는 콜백 함수
	 */
	UFUNCTION()
	void OnAttackWaitStartEventRecived(FGameplayEventData Payload);
	/**
	 * 콤보 입력 대기 종료 처리하는 콜백 함수
	 */
	UFUNCTION()
	void OnAttackWaitEndEventRecived(FGameplayEventData Payload);

private:
	/**
	 * 공격 몽타주를 실행하는 함수
	 *
	 * @param ComboCount 현재 콤보 카운트
	 */
	void ExecuteAttackMontage(int32 ComboCount);
	/**
	 * 콤보 카운트에 따라 몽타주 재생 속도를 조절하는 함수
	 *
	 * @param ComboCount 현재 콤보 카운트
	 * @param PlayRate 재생 속도
	 */
	void SetPlayRateAttackMontage(int32 ComboCount, float PlayRate);
	/** 공격 콤보 카운트를 리셋하는 함수 */
	void ResetAttackComboCount();
	/** 다음 콤보를 처리하는 함수 */
	void ProcessNextCombo();

protected:
	/** 점프 게임플레이 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability", meta = (DisplayName = "Jump Tag"))
	FGameplayTag JumpTag;
	/** 게임플레이 이펙트 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Ability", meta = (DisplayName = "Effect Class"))
	TSubclassOf<UGameplayEffect> EffectClass;
	/** 현재 공격 상태 */
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Ability", meta = (DisplayName = "Current Attack State"))
	ERagnarokAttackState CurAttackState = ERagnarokAttackState::ERAS_None;

private:
	/** 약공격 애니메이션 몽타주 */
	UPROPERTY(EditAnywhere, Category = "KratosLightAttack|Animation", meta = (DisplayName = "Attack Montage"))
	UAnimMontage* AttackMontage = nullptr;

	/** 콤보 카운트별 몽타주 섹션 맵 */
	//UPROPERTY(EditAnywhere, Category = "KratosLightAttack|Combo", meta = (DisplayName = "Combo Section Map"))
	//TMap<int32, FName> ComboSectionMap;

	UPROPERTY(EditAnywhere, Category = "KratosLightAttackAbility")
	TArray<FKratosAttackData> KratosAttackDataArray;

	/** 크레토스 무기 참조 */
	UPROPERTY()
	AKratosWeapon* KratosWeapon = nullptr;

	/** 공격 몽타주 재생 태스크 */
	UAbilityTask_PlayMontageAndWait* AttackMontageTask = nullptr;
	/** 게임플레이 이벤트 대기 태스크 */
	UAbilityTask_WaitGameplayEvent* WaitEventTask = nullptr;
	/** 공격 대기 시작 태스크 */
	UAbilityTask_WaitGameplayEvent* AttackWaitStartTask = nullptr;
	/** 공격 대기 종료 태스크 */
	UAbilityTask_WaitGameplayEvent* AttackWaitEndTask = nullptr;
	/** 공격 대기 종료 태스크 */
	URKAbilityTask_RotateToCamera* RotateToCameraTask = nullptr;

	/** 타이머 핸들 */
	FTimerHandle TimerHandle;
	/** 공격 대기 타이머 핸들 */
	FTimerHandle AttackWaitTimerHandle;
	/** 콤보별 넉백 파워 맵 */
	TMap<int32, float> LaunchPowerMap;
	/** 최대 콤보 카운트 */
	int32 MaxComboCount = 4;
	/** 현재 콤보 카운트 */
	int32 CurComboCount = 1;
	/** 콤보 입력 대기 시간 */
	float ComboWaitDuration = 1.0f;
	/** 다음 콤보 예약 여부 */
	bool bReserveComboAttack = false;
	/** 마지막 공격 반응 게임 태그 */
	FGameplayTag RecentAttackReactTag;
};
