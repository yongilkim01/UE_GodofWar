// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

URagnarokAttributeSet::URagnarokAttributeSet()
{
	InitCurrentHealth(1.0f);
	InitMaxHealth(1.0f);
	InitCurrentRage(1.0f);
	InitMaxRage(1.0f);
	InitAttackPower(1.0f);
	InitDefensePower(1.0f);
}

void URagnarokAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (GetCurrentHealthAttribute() == Data.EvaluatedData.Attribute)
	{
		const float NewHealth = FMath::Clamp(GetCurrentHealth(), 0.0f, GetMaxHealth());

		SetCurrentHealth(NewHealth);
	}

	if (GetCurrentRageAttribute() == Data.EvaluatedData.Attribute)
	{
		const float NewRage = FMath::Clamp(GetCurrentRage(), 0.0f, GetMaxRage());

		SetCurrentHealth(NewRage);
	}

	if (GetDamageTakenAttribute() == Data.EvaluatedData.Attribute)
	{
		const float OldHealth = GetCurrentHealth();
		const float Damage = GetDamageTaken();
		const float CalcHealth = FMath::Clamp(OldHealth - Damage, 0.0f, GetMaxHealth());

		SetCurrentHealth(CalcHealth);

		const FString DebugMsg = FString::Printf
		(
			TEXT("Old health : %f, Damage : %f, CalcHealth : %f"),
			OldHealth, 
			Damage,
			CalcHealth
		);

		Debug::Print(DebugMsg, FColor::Green);
	}
}
