// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Components/LevelAssetManagerComponent.h"

#include "Core/Types/RagnarokTypes.h"
#include "Manager/Global/RagnarokAssetManager.h"
#include "Data/Kratos/CharacterPrimaryAssetKratos.h"

// Sets default values for this component's properties
ULevelAssetManagerComponent::ULevelAssetManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULevelAssetManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	URagnarokAssetManager::Get().LoadAssetData<UCharacterPrimaryAssetKratos>(EPrimaryAssetType::EPT_Chracter_Kratos);

	
}


// Called every frame
void ULevelAssetManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

