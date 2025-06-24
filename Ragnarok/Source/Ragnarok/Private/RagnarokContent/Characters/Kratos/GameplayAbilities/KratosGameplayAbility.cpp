// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"

#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"

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
		KratosController = Cast<AKratosController>(CurrentActorInfo->AvatarActor);;
	}

	return KratosController.Get();
}

UKratosCombatComponent* UKratosGameplayAbility::GetKratosCombatComponent()
{
	return GetKratosFromActorInfo()->GetKratosCombatComponent();
}
