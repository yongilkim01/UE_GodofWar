// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"
#include "GenericTeamAgentInterface.h"

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
