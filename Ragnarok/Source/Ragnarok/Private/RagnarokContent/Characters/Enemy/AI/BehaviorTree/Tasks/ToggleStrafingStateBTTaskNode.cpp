// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/AI/BehaviorTree/Tasks/ToggleStrafingStateBTTaskNode.h"
#include "RagnarokContent/Characters/Enemy/Base/EnemyCharacter.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokAbilityFunctionLibrary.h"

#include "RagnarokContent/Characters/Enemy/EnemyGameplayTags.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UToggleStrafingStateBTTaskNode::ExecuteEnemyTask(AEnemyCharacter* EnemyCharacter)
{
	if (true == bEnableStrafing)
	{
		EnemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		if (true == bChangeMaxWalkSpeed)
		{
			EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = StrafingWalkSpeed;
		}

		URagnarokAbilityFunctionLibrary::AddGameplayTagToActor(EnemyCharacter, EnemyGameplayTags::Enemy_Status_Strafing);
	}
	else
	{
		EnemyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		
		float DefaultMaxWalkSpeed = 0.0f;

		if (AAIController* AIController = Cast<AAIController>(EnemyCharacter->GetController()))
		{
			if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
			{
				DefaultMaxWalkSpeed = BlackboardComp->GetValueAsFloat(DefaultMaxWalkSpeedKey.SelectedKeyName);
			}
		}
		
		if (DefaultMaxWalkSpeed != EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed)
		{
			EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
		}

		URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(EnemyCharacter, EnemyGameplayTags::Enemy_Status_Strafing);

	}

	return EBTNodeResult::Succeeded;
}
