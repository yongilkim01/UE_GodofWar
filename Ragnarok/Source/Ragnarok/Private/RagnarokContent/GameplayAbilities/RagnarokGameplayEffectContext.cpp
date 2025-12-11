// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/GameplayAbilities/RagnarokGameplayEffectContext.h"

bool FRagnarokGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSucess)
{
	Super::NetSerialize(Ar, Map, bOutSucess);
	Ar << AttackTypeTag;
	Ar << HitDirection;
	return true;
}
