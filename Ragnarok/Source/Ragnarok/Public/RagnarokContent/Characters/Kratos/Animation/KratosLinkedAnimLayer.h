// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Animation/RagnarokAnimInstance.h"
#include "KratosLinkedAnimLayer.generated.h"

class UKratosAnimInstance;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosLinkedAnimLayer : public URagnarokAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (NotBlueprintThreadSafe))
	UKratosAnimInstance* GetKratosAnimInstance() const;
};
