// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTServices/MotionWrapAttackTargetBTService.h"
#include "RagnarokContent/Characters/Enemy/Base/EnemyCharacter.h"

#include "AIController.h"
#include "MotionWarpingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


UMotionWrapAttackTargetBTService::UMotionWrapAttackTargetBTService()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorKey), AActor::StaticClass());
}

void UMotionWrapAttackTargetBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (nullptr == EnemyCharacter)
	{
		if (AAIController* AIController = OwnerComp.GetAIOwner())
		{
			if (APawn* OwnerPawn = AIController->GetPawn())
			{
				EnemyCharacter = Cast<AEnemyCharacter>(OwnerPawn);
			}
		}
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (nullptr != BlackboardComp)
	{
		UObject* TargetActorObject = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);
		AActor* TargetActor = Cast<AActor>(TargetActorObject);

		if (nullptr != TargetActor)
		{
			EnemyCharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				FName("AttackTarget"), 
				TargetActor->GetActorLocation());
		}
	}
}
	