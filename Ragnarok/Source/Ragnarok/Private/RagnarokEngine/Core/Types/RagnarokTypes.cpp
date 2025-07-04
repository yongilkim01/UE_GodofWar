// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokGameplayAbility.h"

bool FRagnarokbilitySet::IsValid() const
{
	return true == InputTag.IsValid() && nullptr != Ability;
}
