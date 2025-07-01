// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/GameplayAbilities/RagnarokAbilityFunctionLibrary.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

URagnarokAbilitySystemComponent* URagnarokAbilityFunctionLibrary::GetRagnarokASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<URagnarokAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void URagnarokAbilityFunctionLibrary::AddGameplayTagToActor(AActor* InActor, FGameplayTag TagToAdd)
{
	URagnarokAbilitySystemComponent* ASC = GetRagnarokASCFromActor(InActor);

	if (false == ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(AActor* InActor, FGameplayTag TagToRemove)
{
	URagnarokAbilitySystemComponent* ASC = GetRagnarokASCFromActor(InActor);

	if (true == ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->AddLooseGameplayTag(TagToRemove);
	}
}

bool URagnarokAbilityFunctionLibrary::HasActorGameplayTag(AActor* InActor, FGameplayTag TagToFind)
{
	URagnarokAbilitySystemComponent* ASC = GetRagnarokASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToFind);
}
