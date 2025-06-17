// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/GameFramework/RagnarokGameState.h"

#include "RagnarokEngine/AssetSystem/LevelAssetManagerComponent.h"

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
