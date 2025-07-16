// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTServices/MotionWrapAttackTargetBTService.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyCharacter.h"

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
			if (APawn* ControlledPawn = AIController->GetPawn())
			{
				EnemyCharacter = Cast<AEnemyCharacter>(ControlledPawn);
			}
		}
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (nullptr != BlackboardComp)
	{
		UObject* ActorObject = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);
		AActor* TargetActor = Cast<AActor>(ActorObject);

		if (nullptr != TargetActor)
		{
			EnemyCharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
				FName("AttackTarget"), 
				TargetActor->GetActorLocation());
		}
	}
}
	