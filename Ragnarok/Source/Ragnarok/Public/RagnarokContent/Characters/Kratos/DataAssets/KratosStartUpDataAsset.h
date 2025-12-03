// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Abilities/DataAssets/StartUpDataAsset.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "KratosStartUpDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosStartUpDataAsset : public UStartUpDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(
		URagnarokAbilitySystemComponent* InASC, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ragnarok|StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FRagnarokbilitySet> KratosStartUpAbilitySetArray;
	
};
