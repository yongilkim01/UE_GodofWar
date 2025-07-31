// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Enemy/Base/EnemyCharacter.h"
#include "GruntlingEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API AGruntlingEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AGruntlingEnemyCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
