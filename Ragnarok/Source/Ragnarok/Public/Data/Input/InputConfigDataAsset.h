// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputConfigDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FRagnarokInputActionConfig
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;
};

/**
 * 
 */
UCLASS()
class RAGNAROK_API UInputConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FRagnarokInputActionConfig> InputActionConfigArray;
};
