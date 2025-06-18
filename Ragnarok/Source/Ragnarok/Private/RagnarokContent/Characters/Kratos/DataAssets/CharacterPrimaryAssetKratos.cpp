// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/DataAssets/CharacterPrimaryAssetKratos.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/AssetSystem/RagnarokAssetManager.h"

void UCharacterPrimaryAssetKratos::LoadAsset()
{
	this;
	if (nullptr == KratosSkeletalMesh)
	{
		FSoftObjectPath MeshPath = KratosSkeletalMesh.ToSoftObjectPath();

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			MeshPath,
			FStreamableDelegate::CreateLambda([](){})
		);
	}
}
