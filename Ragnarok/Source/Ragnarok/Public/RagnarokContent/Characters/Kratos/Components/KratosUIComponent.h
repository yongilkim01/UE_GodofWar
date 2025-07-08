// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/UISystem/RagnarokUIComponent.h"
#include "KratosUIComponent.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosUIComponent : public URagnarokUIComponent
{
	GENERATED_BODY()

public:
	FOnValueChangeDelegate OnCurrentRageChanged;
	
};
