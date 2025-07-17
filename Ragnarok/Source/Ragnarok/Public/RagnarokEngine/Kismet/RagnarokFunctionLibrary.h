// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "RagnarokFunctionLibrary.generated.h"

class URagnarokAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Kismet|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintCallable, Category = "Kismet|FunctionLibrary")
	static URagnarokAbilitySystemComponent* GetRagnarokASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Kismet|FunctionLibrary")
	static bool HasActorGameplayTag(AActor* InActor, FGameplayTag TagToCheck);
	
};
