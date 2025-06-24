// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokGameplayAbility.h"
#include "KratosGameplayAbility.generated.h"

class AKratos;
class AKratosController;
class UKratosCombatComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosGameplayAbility : public URagnarokGameplayAbility
{
	GENERATED_BODY()

public:
	AKratos* GetKratosFromActorInfo();
	AKratosController* GetKratosControllerFromActorInfo();
	UKratosCombatComponent* GetKratosCombatComponent();

private:
	TWeakObjectPtr<AKratos> Kratos;
	TWeakObjectPtr<AKratosController> KratosController;
};
