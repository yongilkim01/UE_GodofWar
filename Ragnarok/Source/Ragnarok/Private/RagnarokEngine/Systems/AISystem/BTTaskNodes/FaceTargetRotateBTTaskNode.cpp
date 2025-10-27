// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTTaskNodes/FaceTargetRotateBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"


UFaceTargetRotateBTTaskNode::UFaceTargetRotateBTTaskNode()
{
	NodeName = TEXT("Rotate to Face Target Actor");
	AngleThreshold = 50.0f;
	RotationInterpSpeed = 5.0f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	TargetActorKey.AddObjectFilter(
		this, 
		GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorKey), 
		AActor::StaticClass()
	);
}

void UFaceTargetRotateBTTaskNode::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBData = GetBlackboardAsset())
	{
		TargetActorKey.ResolveSelectedKey(*BBData);
	}

}

uint16 UFaceTargetRotateBTTaskNode::GetInstanceMemorySize() const
{
	return sizeof(FFaceTargetRotateTaskMemory);
}

FString UFaceTargetRotateBTTaskNode::GetStaticDescription() const
{
	const FString KeyDescription = TargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Smoothly rotate to face %s key, angle precision : %s"), *KeyDescription, *FString::SanitizeFloat(AngleThreshold));
}

EBTNodeResult::Type UFaceTargetRotateBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	FFaceTargetRotateTaskMemory* Memory = CastInstanceNodeMemory<FFaceTargetRotateTaskMemory>(NodeMemory);

	Memory->OwnerPawn = ControlledPawn;
	Memory->TargetActor = TargetActor;

	if (false == Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	if (true == HasReachedAnglePercision(ControlledPawn, TargetActor))
	{
		Memory->Reset();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UFaceTargetRotateBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FFaceTargetRotateTaskMemory* Memory = CastInstanceNodeMemory<FFaceTargetRotateTaskMemory>(NodeMemory);

	if (false == Memory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (true == HasReachedAnglePercision(Memory->OwnerPawn.Get(), Memory->TargetActor.Get()))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
			Memory->OwnerPawn->GetActorLocation(), Memory->TargetActor->GetActorLocation());
		const FRotator TargetRotation = FMath::RInterpTo(
			Memory->OwnerPawn->GetActorRotation(), 
			LookAtRotation, DeltaSeconds, 
			RotationInterpSpeed);

		Memory->OwnerPawn->SetActorRotation(TargetRotation);
	}
}

bool UFaceTargetRotateBTTaskNode::HasReachedAnglePercision(APawn* QueryPawn, AActor* TargetActor) const
{
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	const FVector OwnerToTargetNormalVector = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalVector);
	const float Angle = UKismetMathLibrary::DegAcos(DotResult);

	return Angle <= AngleThreshold;
}
