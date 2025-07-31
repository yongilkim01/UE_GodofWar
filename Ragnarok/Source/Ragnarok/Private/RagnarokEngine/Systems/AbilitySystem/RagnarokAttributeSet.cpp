// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAttributeSet.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilityFunctionLibrary.h"
#include "GameplayEffectExtension.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"

#include "RagnarokEngine/Systems/UISystem/UIInterface.h"
#include "RagnarokEngine/Systems/UISystem/RagnarokUIComponent.h"

#include "RagnarokContent/Characters/Kratos/Components/KratosUIComponent.h"

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
	if (false == UIInterfacePtr.IsValid())
	{
		UIInterfacePtr = TWeakInterfacePtr<IUIInterface>(Data.Target.GetAvatarActor());
	}

	checkf(UIInterfacePtr.IsValid(), TEXT("Not implement IUIInterface"));

	URagnarokUIComponent* UIComponent = UIInterfacePtr->GetUIComponent();

	checkf(UIComponent, TEXT("Not implement URagnarokUIComponent"));

	if (GetCurrentHealthAttribute() == Data.EvaluatedData.Attribute)
	{
		const float NewHealth = FMath::Clamp(GetCurrentHealth(), 0.0f, GetMaxHealth());

		SetCurrentHealth(NewHealth);

		float Test = GetCurrentHealth() / GetMaxHealth();

		UIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	if (GetCurrentRageAttribute() == Data.EvaluatedData.Attribute)
	{
		const float NewRage = FMath::Clamp(GetCurrentRage(), 0.0f, GetMaxRage());

		SetCurrentRage(NewRage);

		if (UKratosUIComponent* KratosUIComponent = Cast<UKratosUIComponent>(UIComponent))
		{
			KratosUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}

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

		UIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		if (0.0f == CalcHealth)
		{
			URagnarokAbilityFunctionLibrary::AddGameplayTagToActor(
				Data.Target.GetAvatarActor(),
				RagnarokGameplayTags::Global_State_Dead
			);
		}
	}
}
