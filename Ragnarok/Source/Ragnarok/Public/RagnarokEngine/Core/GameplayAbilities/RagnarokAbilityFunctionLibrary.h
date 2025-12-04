// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "RagnarokAbilityFunctionLibrary.generated.h"

class URagnarokAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokAbilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static URagnarokAbilitySystemComponent* GetRagnarokASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Ragnarok|AbilitySystem")
	static void AddGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Ragnarok|AbilitySystem")
	static void RemoveGameplayTagToActor(AActor* InActor, FGameplayTag TagToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Ragnarok|AbilitySystem")
	static bool HasActorGameplayTag(AActor* InActor, FGameplayTag TagToFind);
};
