// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokAIController.h"
#include "EnemyAIController.generated.h"

/**
 * Enemy AI Controller 클래스
 */
UCLASS()
class RAGNAROK_API AEnemyAIController : public ARagnarokAIController
{
	GENERATED_BODY()

public:
	/** 생성자 */
	AEnemyAIController(const FObjectInitializer& ObjectInitializer);

	/** 컴포넌트 기본값 세팅 */
	virtual void BeginPlay() override;
	/** Behavior Tree를 실행하고 Blackboard에 기본값을 세팅 */
	virtual void OnPossess(APawn* InPawn) override;
	/** 함수 인자로 들어온 액터의 TeamID를 비교하여 적대/우호 관계를 반환하는 함수 */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	/**
	 * AI Perception Component 타겟 액터 업데이트 바인딩 함수
	 *
	 * @param Actor 감지 액터
	 * @param Stimulus AI 자극 구조체 정보
	 */
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	/** AI 인지 컴포넌트 멤버 변수*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "AI Perception Component"))
	UAIPerceptionComponent* AIPerceptionComponent = nullptr;

	/** 시야 감지 설정 객체 멤버 변수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "AI Sense Config Sight"))
	UAISenseConfig_Sight* AISenseConfig_Sight = nullptr;

	/** 실행할 Behavior Tree 객체 멤버 변수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Enemy Behavior Tree"))
	UBehaviorTree* EnemyBehaviorTree = nullptr;
};
