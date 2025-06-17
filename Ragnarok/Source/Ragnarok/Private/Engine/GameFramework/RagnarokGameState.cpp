// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/GameFramework/RagnarokGameState.h"

#include "Manager/Components/LevelAssetManagerComponent.h"

ARagnarokGameState::ARagnarokGameState()
{
	LevelAssetManager = CreateDefaultSubobject<ULevelAssetManagerComponent>(TEXT("LevelAssetManager"));
}

void ARagnarokGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ARagnarokGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
