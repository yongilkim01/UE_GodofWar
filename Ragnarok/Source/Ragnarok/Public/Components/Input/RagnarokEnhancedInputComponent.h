// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Data/Input/InputConfigDataAsset.h"
#include "RagnarokEnhancedInputComponent.generated.h"

class UInputConfigDataAsset;

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserObject, typename CallbackFunc>
	void BindNativeInputAction(const UInputConfigDataAsset* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* MappingContext, CallbackFunc Func);
};

template<class UserObject, typename CallbackFunc>
inline void URagnarokEnhancedInputComponent::BindNativeInputAction(const UInputConfigDataAsset* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* MappingContext, CallbackFunc Func)
{
	checkf(InInputConfig, TEXT("Input config data asset is null"));

	UInputAction* FoundInputAction = InInputConfig->FindNativeInputActionByTag(InInputTag);

	if (nullptr != FoundInputAction)
	{
		BindAction(FoundInputAction, TriggerEvent, MappingContext, Func);
	}
}
