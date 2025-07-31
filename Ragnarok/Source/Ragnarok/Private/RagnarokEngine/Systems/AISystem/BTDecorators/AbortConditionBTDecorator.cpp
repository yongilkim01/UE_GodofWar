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
		Debug::Print(TEXT("UAbortConditionBTDecorator::BlackboardComponent is nullptr!"), FColor::Yellow);
		return false;
	}

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (nullptr == ControlledPawn)
	{
		Debug::Print(TEXT("UAbortConditionBTDecorator::ControlledPawn is nullptr!"), FColor::Yellow);
		return false;
	}

	UObject* ActorObject = BlackboardComponent->GetValueAsObject(InTargetActorKey.SelectedKeyName);

	if (nullptr == ActorObject)
	{
		Debug::Print(TEXT("UAbortConditionBTDecorator::ActorObject is nullptr!"), FColor::Yellow);
		return false;
	}
		
	float DistanceToTarget = BlackboardComponent->GetValueAsFloat(DistanceToTargetKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);
	AActor* OwnerAIActor = Cast<AActor>(ControlledPawn);

	bool bTargetActorDead = false;
	bool bOwnerAIActorDead = false;

	if (true == URagnarokFunctionLibrary::HasActorGameplayTag(TargetActor, RagnarokGameplayTags::Global_State_Dead))
	{
		bTargetActorDead = true;
	}

	if (true == URagnarokFunctionLibrary::HasActorGameplayTag(OwnerAIActor, RagnarokGameplayTags::Global_State_Dead))
	{
		bOwnerAIActorDead = true;
	}

	return true == bTargetActorDead ||
		true == bOwnerAIActorDead ||
		FMath::IsNearlyEqual(DistanceToTarget, 0.0f);
}