// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AbilitySystem/EffectExecCalcs/DamageTakenEffectExecCalc.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAttributeSet.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

struct FRagnarokDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FRagnarokDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URagnarokAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(URagnarokAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(URagnarokAttributeSet, DamageTaken, Target, false)
	}
};

static const FRagnarokDamageCapture& GetRagnarokDamageCapture()
{
	static FRagnarokDamageCapture RagnarokDamageCapture;
	return RagnarokDamageCapture;
}

UDamageTakenEffectExecCalc::UDamageTakenEffectExecCalc()
{
	//FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	//	URagnarokAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(URagnarokAttributeSet, AttackPower)
	//);

	//FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	//	AttackPowerProperty,
	//	EGameplayEffectAttributeCaptureSource::Source,
	//	false
	//);

	//RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

	RelevantAttributesToCapture.Add(GetRagnarokDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetRagnarokDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetRagnarokDamageCapture().DamageTakenDef);
}

void UDamageTakenEffectExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float SourceAttackPower = 0.0f;
	float TargetDefensePower = 0.0f;
	float Damage = 0.0f;
	int32 LightAttackComboCount = 0;
	int32 HeavyAttackComboCount = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetRagnarokDamageCapture().AttackPowerDef, 
		EvaluateParameters,
		SourceAttackPower
	);

	//Debug::Print(TEXT("Soure Attack Power"), SourceAttackPower);

	// GameAbility에서 전달한 무기의 기본 데미지를 계산

	for (const TPair<FGameplayTag, float>& TagMagniture : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (true == TagMagniture.Key.MatchesTagExact(RagnarokGameplayTags::Global_SetByCaller_BaseDamage))
		{
			Damage = TagMagniture.Value;
			//Debug::Print(TEXT("Damage"), Damage);
		}

		if (TagMagniture.Key.MatchesTagExact(KratosGameplayTags::Kratos_SetByCaller_AttackType_Light))
		{
			LightAttackComboCount = TagMagniture.Value;
			//Debug::Print(TEXT("Light Attack Combo"), LightAttackComboCount);
		}

		if (TagMagniture.Key.MatchesTagExact(KratosGameplayTags::Kratos_SetByCaller_AttackType_Heavy))
		{
			HeavyAttackComboCount = TagMagniture.Value;
			//Debug::Print(TEXT("Heavy Attack Combo"), HeavyAttackComboCount);
		}
	}

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetRagnarokDamageCapture().DefensePowerDef,
		EvaluateParameters,
		TargetDefensePower
	);

	//Debug::Print(TEXT("Target Defense Power"), TargetDefensePower);


	if (0 != LightAttackComboCount)
	{
		const float IncreaseDamage = (LightAttackComboCount - 1) * 0.05f + 1.0f;
		Damage *= IncreaseDamage;
		//Debug::Print(TEXT("Increase Light Attack Damage"), Damage);

	}

	if (0 != HeavyAttackComboCount)
	{
		const float HeavyIncreaseDamage = HeavyAttackComboCount * 0.15f + 1.0f;
		Damage *= HeavyIncreaseDamage;
		//Debug::Print(TEXT("Increase Heavy Attack Damage"), Damage);
	}

	const float FinalDamage = Damage * SourceAttackPower / TargetDefensePower;

	//Debug::Print(TEXT("Final Attack Damage"), FinalDamage);

	if (FinalDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier
		(
			FGameplayModifierEvaluatedData
			(
				GetRagnarokDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamage
			)	
		);
	}
}
