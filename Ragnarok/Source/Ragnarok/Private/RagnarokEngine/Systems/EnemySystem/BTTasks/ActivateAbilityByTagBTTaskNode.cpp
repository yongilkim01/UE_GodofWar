// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/EnemySystem/BTTasks/ActivateAbilityByTagBTTaskNode.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyCharacter.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

EBTNodeResult::Type UActivateAbilityByTagBTTaskNode::ExecuteEnemyTask(AEnemyCharacter* EnemyCharacter)
{
	URagnarokAbilitySystemComponent* RagnarokASC = Cast<URagnarokAbilitySystemComponent>(EnemyCharacter->GetAbilitySystemComponent());

	check(RagnarokASC);

	return RagnarokASC->TryActivateAbilityByTag(TagToActivate) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
