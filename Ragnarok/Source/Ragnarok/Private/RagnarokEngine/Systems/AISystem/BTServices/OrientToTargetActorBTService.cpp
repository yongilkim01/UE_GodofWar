// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTServices/OrientToTargetActorBTService.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UOrientToTargetActorBTService::UOrientToTargetActorBTService()
{
	NodeName = TEXT("Orient Rotation To Target Actor");
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	RotationSpeed = 5.0f;
	Interval = 0.0f;
	RandomDeviation = 0.0f;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorKey), AActor::StaticClass());
}

void UOrientToTargetActorBTService::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BlackboardData = GetBlackboardAsset())
	{
		TargetActorKey.ResolveSelectedKey(*BlackboardData);
	}
}

FString UOrientToTargetActorBTService::GetStaticDescription() const
{
	const FString KeyDescription = TargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UOrientToTargetActorBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (nullptr != OwningPawn && nullptr != TargetActor)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationSpeed);

		OwningPawn->SetActorRotation(TargetRot);
	}
}

//FString UOrientToTargetActorBTService::GetStaticDescription() const
//{
//	const FString KeyDescription = TargetActorKey.SelectedKeyName.ToString();
//	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, GetStaticServiceDescription());
//}
