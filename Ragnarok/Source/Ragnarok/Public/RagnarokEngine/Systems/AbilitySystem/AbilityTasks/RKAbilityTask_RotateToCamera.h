// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RKAbilityTask_RotateToCamera.generated.h"

class URagnarokGameplayAbility;

/**
 * 카메라 방향으로 회전시키는 AbilityTask
 */
UCLASS()
class RAGNAROK_API URKAbilityTask_RotateToCamera : public UAbilityTask
{
	GENERATED_BODY()

public:
	/**
	 * 카메라 방향으로 회전하는 AbilityTask를 생성하는 함수
	 *
	 * @param OwnerAbility 이 Task를 소유하는 Ability 클래스
	 * @param TaskName Task 인스턴스 이름
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilitySystem|AbilityTasks", meta = (HidePin = "OwnerAbility", DefaultToSelf = "OwnerAbility", BlueprintInternalUseOnly = "TRUE"))
	static URKAbilityTask_RotateToCamera* CreateRotateToCameraTask(
		URagnarokGameplayAbility* OwnerAbility,
		FName TaskName
	);

	//~ Begin UAbilityTask Interface.
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	//~ End UAbilityTask Interface.

private:
	/** URagnarokGameplayAbility 참조 변수 */
	TWeakObjectPtr<URagnarokGameplayAbility> RagnarokAbility;
};
