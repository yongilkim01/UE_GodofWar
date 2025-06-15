// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/RagnarokGameModeBase.h"

#include "Core/Types/RagnarokTypes.h"
#include "Manager/Global/RagnarokAssetManager.h"


ARagnarokGameModeBase::ARagnarokGameModeBase()
{
}

void ARagnarokGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//URagnarokAssetManager::Get().LoadPrimaryAssetData(EPrimaryAssetType::EPT_Item_LeviathanAxe);
}

void ARagnarokGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
