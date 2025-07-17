// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

#include "GenericTeamAgentInterface.h"
#include "AbilitySystemBlueprintLibrary.h"

bool URagnarokFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(nullptr != QueryPawn && nullptr != TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (nullptr != QueryTeamAgent && nullptr != TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

URagnarokAbilitySystemComponent* URagnarokFunctionLibrary::GetRagnarokASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<URagnarokAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}


bool URagnarokFunctionLibrary::HasActorGameplayTag(AActor* InActor, FGameplayTag TagToCheck)
{
	URagnarokAbilitySystemComponent* ASC = GetRagnarokASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}
