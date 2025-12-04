// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/AI/BehaviorTree/RagnarokBTTaskNode.h"
#include "EnemyBTTaskNode.generated.h"

class AEnemyCharacter;

/**
 * UEnemyBTTaskNode의 인스턴스별 데이터를 저장하는 구조체
 * CachedEnemyCharacter AEnemyCharacterd의 포인터를 캐시하여 매 프레임 조회 비용을 절감
 */
struct FEnemyTaskMemory
{
	AEnemyCharacter* CachedEnemyCharacter;
};

/**
 * EnemyCharacter들이 사용하는 BTTaskNode들의 부모 클래스
 */
UCLASS()
class RAGNAROK_API UEnemyBTTaskNode : public URagnarokBTTaskNode
{
	GENERATED_BODY()

public:
	/** 생성자 */
	UEnemyBTTaskNode();

	/** 
	 * 적 캐릭터를 대상으로 태스크를 실행하는 가상 함수 
	 * 자식 클래스에서 이 함수를 오버라이드하여 실제 태스크 로직을 구현하는 함수
	 * 
	 * @param EnemyCharacter 태스크를 실행할 적 캐릭터
	 * @return 태스크 실행 결과 (Succeeded, Failed, InProgress 등)
	 */
	virtual EBTNodeResult::Type ExecuteEnemyTask(AEnemyCharacter* EnemyCharacter);

protected:
	/** AIController에서 EnemyCharacter를 가져와 캐싱을 하고 ExecuteEnemyTask를 호출하는 함수 */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** FEnemyTaskMemory 메모리 크기를 반환하는 함수 */
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FEnemyTaskMemory); }
};
