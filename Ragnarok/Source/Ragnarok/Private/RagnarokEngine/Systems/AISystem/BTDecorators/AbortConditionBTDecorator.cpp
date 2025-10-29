// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTDecorators/AbortConditionBTDecorator.h"

#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UAbortConditionBTDecorator::UAbortConditionBTDecorator()
{
	NodeName = "Am I dead?";

}

bool UAbortConditionBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (nullptr == BlackboardComponent)
	{
		Debug::Print(TEXT("UAbortConditionBTDecorator::BlackboardComponent is nullptr"), FColor::Yellow);
		return false;
	}

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (nullptr == OwnerPawn)
	{
		Debug::Print(TEXT("UAbortConditionBTDecorator::ControlledPawn is nullptr"), FColor::Yellow);
		return false;
	}

	UObject* TargetActorObject = BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName);

	if (nullptr == TargetActorObject)
	{
		Debug::Print(TEXT("UAbortConditionBTDecorator::ActorObject is nullptr"), FColor::Yellow);
		return false;
	}
		
	float DistanceToTarget = BlackboardComponent->GetValueAsFloat(DistanceToTargetKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetActorObject);
	AActor* OwnerActor = Cast<AActor>(OwnerPawn);

	bool bTargetActorDead = false;
	bool bOwnerActorDead = false;

	if (true == URagnarokFunctionLibrary::HasActorGameplayTag(TargetActor, RagnarokGameplayTags::Global_State_Dead))
	{
		bTargetActorDead = true;
	}

	if (true == URagnarokFunctionLibrary::HasActorGameplayTag(OwnerActor, RagnarokGameplayTags::Global_State_Dead))
	{
		bOwnerActorDead = true;
	}

	// 타겟이 죽거나 오너 액터가 죽거나 거리가 0일 경우 true 반환
	return true == bTargetActorDead || true == bOwnerActorDead || FMath::IsNearlyEqual(DistanceToTarget, 0.0f);
}