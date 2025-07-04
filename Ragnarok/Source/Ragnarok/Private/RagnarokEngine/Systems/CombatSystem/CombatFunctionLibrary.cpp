// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/CombatSystem/CombatFunctionLibrary.h"
#include "RagnarokEngine/Systems/CombatSystem/CombatInterface.h"

UCombatComponent* UCombatFunctionLibrary::GetCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(InActor))
	{
		return CombatInterface->GetCombatComponent();
	}

	return nullptr;
}
