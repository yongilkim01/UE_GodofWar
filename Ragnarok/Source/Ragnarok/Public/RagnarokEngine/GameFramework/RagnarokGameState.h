// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RagnarokGameState.generated.h"

class ULevelAssetManagerComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API ARagnarokGameState : public AGameState
{
	GENERATED_BODY()

public:
	ARagnarokGameState();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	ULevelAssetManagerComponent* LevelAssetManager = nullptr;
};
