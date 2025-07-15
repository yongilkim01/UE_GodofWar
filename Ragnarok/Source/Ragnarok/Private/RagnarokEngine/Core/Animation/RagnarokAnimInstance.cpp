// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Core/Animation/RagnarokAnimInstance.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilityFunctionLibrary.h"

bool URagnarokAnimInstance::IsOwnerHaveTag(FGameplayTag FindGameplayTag) const
{
	if (APawn* OwnerPawn = TryGetPawnOwner())
	{
		return URagnarokAbilityFunctionLibrary::HasActorGameplayTag(OwnerPawn, FindGameplayTag);
	}

	return false;
}
