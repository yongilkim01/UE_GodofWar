// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/GameplayAbilities/RagnarokAttributeSet.h"

URagnarokAttributeSet::URagnarokAttributeSet()
{
	InitCurrentHealth(1.0f);
	InitMaxHealth(1.0f);
	InitCurrentRage(1.0f);
	InitMaxRage(1.0f);
	InitAttackPower(1.0f);
	InitDefensePower(1.0f);
}
