// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RagnarokBTService.generated.h"

/**
 * Ragnarok프로젝트에서 BTService 최상위 부모 클래스
 * 일정 주기로 계속 실행되는 노드
 */
UCLASS()
class RAGNAROK_API URagnarokBTService : public UBTService
{
	GENERATED_BODY()

public:
	URagnarokBTService();
};
