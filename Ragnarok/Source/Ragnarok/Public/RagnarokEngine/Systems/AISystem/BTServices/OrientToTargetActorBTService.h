// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTService.h"
#include "OrientToTargetActorBTService.generated.h"

/**
 * 틱마다 블랙보드에서 TargetActor를 가져와 부드럽게 회전시키는 BTService 클래스
 */
UCLASS()
class RAGNAROK_API UOrientToTargetActorBTService : public URagnarokBTService
{
	GENERATED_BODY()

	/** 생성자 */
	UOrientToTargetActorBTService();

	//~ Begin UBTNode Interface.
	/** 비헤이비어 트리 에셋에서 초기화하는 함수 */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	/** 에디터에 표시될 노드 설명을 반환하는 함수 */
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface.

	//~ Begin UBTService Interface.
	/** 블랙보드에서 TargetActor를 가져와 소유 폰을 부드럽게 회전시키는 함수 */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//~ End UBTService Interface.

	/** TargetActor 블랙보드 키 */
	UPROPERTY(EditAnywhere, Category = "Target", meta = (DisplayName = "Target Actor Key"))
	FBlackboardKeySelector TargetActorKey;
	/** 회전 속도 */
	UPROPERTY(EditAnywhere, Category = "Target", meta = (DisplayName = "Rotation Speed"))
	float RotationSpeed = 0.0f;
};
 