// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatFunctionLibrary.generated.h"

class UCombatComponent;

/**
 * CombatComponent 관련 처리 함수 BlueprintFuncionLibrary 클래스
 */
UCLASS()
class RAGNAROK_API UCombatFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UCombatComponent* GetCombatComponentFromActor(AActor* InActor);
};
