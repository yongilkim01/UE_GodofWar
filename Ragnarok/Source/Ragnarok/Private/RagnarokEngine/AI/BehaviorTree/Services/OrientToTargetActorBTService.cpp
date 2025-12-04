// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/AI/BehaviorTree/Services/OrientToTargetActorBTService.h"
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

	UObject* TargetActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetActorObject);

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (nullptr != OwnerPawn && nullptr != TargetActor)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwnerPawn->GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(OwnerPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationSpeed);

		OwnerPawn->SetActorRotation(TargetRot);
	}
}