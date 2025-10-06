// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Camera/RagnarokSpringArmComponent.h"
#include "KratosSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosSpringArmComponent : public URagnarokSpringArmComponent
{
	GENERATED_BODY()

public:
	UKratosSpringArmComponent();

	//~ Begin UActorComponent Interface.
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface.
	
};
