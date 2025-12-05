// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/CombatSystem/CombatFunctionLibrary.h"
#include "RagnarokEngine/Core/Interfaces/CombatInterface.h"

UCombatComponent* UCombatFunctionLibrary::GetCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(InActor))
	{
		return CombatInterface->GetCombatComponent();
	}

	return nullptr;
}
