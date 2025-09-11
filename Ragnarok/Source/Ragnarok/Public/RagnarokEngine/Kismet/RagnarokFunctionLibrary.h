// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
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
	UFUNCTION(BlueprintCallable, Category = "RagnarokKismet|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintCallable, Category = "RagnarokKismet|FunctionLibrary")
	static URagnarokAbilitySystemComponent* GetRagnarokASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "RagnarokKismet|FunctionLibrary")
	static bool HasActorGameplayTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "RagnarokKismet|FunctionLibrary", meta = (DisplayName = "Has Actor Gameplay Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_HasActorGameplayTag(AActor* InActor, FGameplayTag TagToCheck, ERagnarokConfirmType& OutConfirmType);

	UFUNCTION(BlueprintCallable, Category = "RagnarokKismet|FunctionLibrary")
	static void AddGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "RagnarokKismet|FunctionLibrary")
	static void RemoveGameplayTagToActor(AActor* InActor, FGameplayTag TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "RagnarokKismet|FunctionLibrary")
	static void SendGameplayEventToActorWithMagnitude(AActor* InActor, FGameplayTag EventTag, float EventMagnitude);

	UFUNCTION(BlueprintCallable, Category = "RagnarokKismet|FunctionLibrary")
	static void SendGameplayEventToActor(AActor* InActor, FGameplayTag EventTag);
	
};
