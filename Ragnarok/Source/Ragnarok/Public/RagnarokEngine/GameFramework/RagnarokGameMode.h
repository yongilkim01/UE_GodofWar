// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RagnarokGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API ARagnarokGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ARagnarokGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
