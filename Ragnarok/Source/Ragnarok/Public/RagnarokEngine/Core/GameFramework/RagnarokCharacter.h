// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RagnarokEngine/Systems/CombatSystem/CombatInterface.h"
#include "RagnarokEngine/Systems/UISystem/UIInterface.h"
#include "RagnarokCharacter.generated.h"

class URagnarokAbilitySystemComponent;
class URagnarokAttributeSet;
class UStartUpDataAsset;
class UMotionWarpingComponent;

/**
 * 라그나로크 캐릭터 베이스 클래스
 */
UCLASS()
class RAGNAROK_API ARagnarokCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface, public IUIInterface
{
	GENERATED_BODY()

public:
	/** 생성자 */
	ARagnarokCharacter();

	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//~ End AActor Interface.

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface.

	//~ Begin ICombatInterface Interface.
	virtual UCombatComponent* GetCombatComponent() const override;

	/**
	 * 캐릭터가 죽을 때 호출되는 함수
	 *
	 * @param DeathNiagaraEffect 죽음 시 재생할 나이아가라 이펙트
	 */
	virtual void Die(TSoftObjectPtr<UNiagaraSystem> DeathNiagaraEffect) override;
	//~ End ICombatInterface Interface.
	
	//~ Begin IUIInterface Interface.
	virtual URagnarokUIComponent* GetUIComponent() const override;
	//~ End IUIInterface Interface.

	/** 어빌리티 시스템 컴포넌트를 반환하는 함수 */
	FORCEINLINE URagnarokAbilitySystemComponent* GetAbilitySystem() const { return AbilitySystemComponent; }

	/** 모션 워핑 컴포넌트를 반환하는 함수 */
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }

	/** 어트리뷰트 셋을 반환하는 함수 */
	FORCEINLINE URagnarokAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	/** 라그나로크 어빌리티 시스템 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RagnarokCharacter|Components", meta = (DisplayName = "Ability System Component"))
	URagnarokAbilitySystemComponent* AbilitySystemComponent = nullptr;

	/** 모션 워핑 컴포넌트 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RagnarokCharacter|Components", meta = (DisplayName = "Motion Warping Component"))
	UMotionWarpingComponent* MotionWarpingComponent = nullptr;

	/** 라그나로크 어트리뷰트 셋 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RagnarokCharacter|GAS", meta = (DisplayName = "Attribute Set"))
	URagnarokAttributeSet* AttributeSet = nullptr;

	/** 시작 시 초기화 데이터 에셋 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RagnarokCharacter|Data", meta = (DisplayName = "Start Up Data"))
	TSoftObjectPtr<UStartUpDataAsset> StartUpData;

	/** 디버그 메시지 표시 여부 */
	UPROPERTY(EditAnywhere, Category = "RagnarokCharacter|Debug", meta = (DisplayName = "Show Debug Messages"))
	bool bShowDebugMsg = false;
};
