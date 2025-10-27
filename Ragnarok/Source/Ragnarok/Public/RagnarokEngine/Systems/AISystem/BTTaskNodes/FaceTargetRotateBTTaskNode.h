// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTTaskNode.h"
#include "FaceTargetRotateBTTaskNode.generated.h"

/**
 * BTTaskNode의 인스턴스별 데이터를 저장하는 구조체
 */
struct FFaceTargetRotateTaskMemory
{
	/** 소유자 약한 포인터 */
	TWeakObjectPtr<APawn> OwnerPawn;
	/** 타겟 액터 약한 포인터 */
	TWeakObjectPtr<AActor> TargetActor;

	/** 메모리의 유효성을 검사하는 함수 */
	bool IsValid() const 
	{ 
		return true == OwnerPawn.IsValid() && true == TargetActor.IsValid(); 
	}
	/** 메모리를 초기화하는 함수 */
	void Reset()
	{
		OwnerPawn.Reset();
		TargetActor.Reset();
	}
};

/**
 * 블랙보드 TargetActor 향해 회전하는 태스크 노드
 */
UCLASS()
class RAGNAROK_API UFaceTargetRotateBTTaskNode : public URagnarokBTTaskNode
{
	GENERATED_BODY()

public:
	/** 생성자 */
	UFaceTargetRotateBTTaskNode();

	//~ Begin UBTNode Interface.
	/** 비헤이비어 트리 에셋에서 초기화하는 함수 */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	/** 인스턴스 메모리 크기를 반환하는 함수 */
	virtual uint16 GetInstanceMemorySize() const override;
	/** 에디터에 표시될 노드 설명을 반환하는 함수 */
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface.

	//~ Begin UBTTaskNode Interface.
	/** 블랙보드에서 타겟 액터를 가져와 회전을 시작 */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	/** 타겟을 향해 회전하는 함수 */
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//~ End UBTTaskNode Interface.

	/**
	 * 목표 각도에 도달했는지 확인하는 함수
	 *
	 * @param OwnerPawn Blackboard를 사용하고있는 Pawn
	 * @param TargetActor 타겟 액터
	 * @return 목표 각도에 도달했으면 true
	 */
	bool HasReachedAnglePercision(APawn* OwnerPawn, AActor* TargetActor) const;

protected:
	/** 목표 각도 임계값 */
	UPROPERTY(EditAnywhere, Category = "FaceTargetRotate|Settings", meta = (DisplayName = "Angle Threshold"))
	float AngleThreshold = 0.0f;

	/** 회전 보간 속도 */
	UPROPERTY(EditAnywhere, Category = "FaceTargetRotate|Settings", meta = (DisplayName = "Rotation Interpolate Speed"))
	float RotationInterpSpeed = 0.0f;

	/** 바라볼 타겟 블랙보드 키 */
	UPROPERTY(EditAnywhere, Category = "FaceTargetRotate|Blackboard", meta = (DisplayName = "Target Actor Key"))
	FBlackboardKeySelector TargetActorKey;
};
