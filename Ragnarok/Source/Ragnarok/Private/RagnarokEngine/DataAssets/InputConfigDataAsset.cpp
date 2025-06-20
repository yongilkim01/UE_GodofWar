// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/DataAssets/InputConfigDataAsset.h"

UInputAction* UInputConfigDataAsset::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FRagnarokInputActionConfig& InputActionConfig : InputActionConfigArray)
	{
		if (InputActionConfig.InputTag == InInputTag && nullptr != InputActionConfig.InputAction)
		{
			return InputActionConfig.InputAction;
		}
	}

	return nullptr;
}
