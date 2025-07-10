// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "KratosController.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API AKratosController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AKratosController();

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface.

private:
	FGenericTeamId KratosTeamID;
	
};
