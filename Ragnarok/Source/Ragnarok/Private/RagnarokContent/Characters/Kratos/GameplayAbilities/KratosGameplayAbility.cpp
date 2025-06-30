// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"

#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"

void UKratosGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

AKratos* UKratosGameplayAbility::GetKratosFromActorInfo()
{
	if (false == Kratos.IsValid())
	{
		Kratos = Cast<AKratos>(CurrentActorInfo->AvatarActor);;
	}

	return Kratos.Get();
}

AKratosController* UKratosGameplayAbility::GetKratosControllerFromActorInfo()
{
	if (false == KratosController.IsValid())
	{
		APawn* Pawn = Cast<APawn>(CurrentActorInfo->AvatarActor);

		if (nullptr != Pawn)
		{
			KratosController = Cast<AKratosController>(Pawn->GetController());
		}
	}

	return KratosController.Get();
}

UKratosCombatComponent* UKratosGameplayAbility::GetKratosCombatComponent()
{
	return GetKratosFromActorInfo()->GetKratosCombatComponent();
}
