// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTDecorators/HasGameplayTagBTDecorator.h"

#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UHasGameplayTagBTDecorator::UHasGameplayTagBTDecorator()
{
	NodeName = "Has GameplayTag?";

}

bool UHasGameplayTagBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (nullptr == BlackboardComponent)
	{
		Debug::Print(TEXT("UHasGameplayTagBTDecorator::BlackboardComponent is nullptr!"), FColor::Yellow);
		return false;
	}

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (nullptr == ControlledPawn)
	{
		Debug::Print(TEXT("UHasGameplayTagBTDecorator::ControlledPawn is nullptr!"), FColor::Yellow);
		return false;
	}

	UObject* ActorObject = BlackboardComponent->GetValueAsObject(ActorToCheckKey.SelectedKeyName);
	AActor* CheckActor = Cast<AActor>(ActorObject);

	if (true == URagnarokFunctionLibrary::HasActorGameplayTag(CheckActor, TagToCheck))
	{
		return !bInverseConditionCheck;
	}
	else
	{
		return bInverseConditionCheck;
	}
}
