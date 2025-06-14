// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RagnarokGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API ARagnarokGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARagnarokGameModeBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
