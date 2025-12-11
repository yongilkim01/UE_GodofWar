// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/GameplayAbilities/RagnarokAbilitySystemGlobals.h"
#include "RagnarokContent/GameplayAbilities/RagnarokGameplayEffectContext.h"

FGameplayEffectContext* URagnarokAbilitySystemGlobals::AllocGameplayEffectContext() const
{
    return new FRagnarokGameplayEffectContext();
}
