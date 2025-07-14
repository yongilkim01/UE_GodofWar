// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyBTTaskNode.h"
#include "ToggleStrafingStateBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UToggleStrafingStateBTTaskNode : public UEnemyBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteEnemyTask(AEnemyCharacter* EnemyCharacter) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Enemy")
	bool bEnable = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Enemy")
	bool bChangeMaxWalkSpeed = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Enemy")
	float StrafingWalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Enemy")
	FBlackboardKeySelector DefaultMaxWalkSpeedKey;
};
