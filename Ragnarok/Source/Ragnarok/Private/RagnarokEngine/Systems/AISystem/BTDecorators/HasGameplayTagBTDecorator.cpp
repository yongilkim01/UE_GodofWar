// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTDecorators/HasGameplayTagBTDecorator.h"

#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UHasGameplayTagBTDecorator::UHasGameplayTagBTDecorator()
{
	NodeName = "Has GameplayTag?";

}

bool UHasGameplayTagBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (nullptr == BlackboardComp)
	{
		Debug::Print(TEXT("UHasGameplayTagBTDecorator::BlackboardComp is nullptr"), FColor::Yellow);
		return false;
	}

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (nullptr == OwnerPawn)
	{
		Debug::Print(TEXT("UHasGameplayTagBTDecorator::BlackboardComp is nullptr"), FColor::Yellow);
		return false;
	}

	UObject* CheckActorObject = BlackboardComp->GetValueAsObject(CheckActorKey.SelectedKeyName);
	AActor* CheckActor = Cast<AActor>(CheckActorObject);

	if (true == URagnarokFunctionLibrary::HasActorGameplayTag(CheckActor, TagToCheck))
	{
		return !bInverseConditionCheck;
	}
	else
	{
		return bInverseConditionCheck;
	}
}
