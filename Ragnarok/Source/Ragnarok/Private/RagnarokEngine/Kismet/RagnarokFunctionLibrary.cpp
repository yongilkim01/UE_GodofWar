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

void URagnarokFunctionLibrary::BP_HasActorGameplayTag(AActor* InActor, FGameplayTag TagToCheck, ERagnarokConfirmType& OutConfirmType)
{
	OutConfirmType = HasActorGameplayTag(InActor, TagToCheck) ? ERagnarokConfirmType::ERCT_Yes : ERagnarokConfirmType::ERCT_No;
}

void URagnarokFunctionLibrary::AddGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd)
{
	URagnarokAbilitySystemComponent* ASC = GetRagnarokASCFromActor(InActor);

	if (false == ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void URagnarokFunctionLibrary::RemoveGameplayTagToActor(AActor* InActor, FGameplayTag TagToRemove)
{
	URagnarokAbilitySystemComponent* ASC = GetRagnarokASCFromActor(InActor);

	if (true == ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

void URagnarokFunctionLibrary::SendGameplayEventToActorWithMagnitude(AActor* InActor, FGameplayTag EventTag, float EventMagnitude)
{
	if (nullptr == InActor)
	{
		return;
	}

	FGameplayEventData EventData;
	EventData.EventTag = EventTag;
	EventData.EventMagnitude = EventMagnitude;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InActor, EventTag, EventData);
}

void URagnarokFunctionLibrary::SendGameplayEventToActor(AActor* InActor, FGameplayTag EventTag)
{
	if (nullptr == InActor)
	{
		return;
	}

	FGameplayEventData EventData;
	EventData.EventTag = EventTag;
	EventData.Instigator = InActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(InActor, EventTag, EventData);
}
