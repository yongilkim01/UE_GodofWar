// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTTaskNodes/FaceTargetRotateBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"


UFaceTargetRotateBTTaskNode::UFaceTargetRotateBTTaskNode()
{
	NodeName = TEXT("Rotate to Face Target Actor");
	AnglePrecision = 10.0f;
	RotationInterpolateSpeed = 5.0f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	TargetToFaceKey.AddObjectFilter(
		this, 
		GET_MEMBER_NAME_CHECKED(ThisClass, TargetToFaceKey), 
		AActor::StaticClass()
	);

}

void UFaceTargetRotateBTTaskNode::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBData = GetBlackboardAsset())
	{
		TargetToFaceKey.ResolveSelectedKey(*BBData);
	}

}

uint16 UFaceTargetRotateBTTaskNode::GetInstanceMemorySize() const
{
	return sizeof(FFaceTargetRotateTaskMemory);
}

FString UFaceTargetRotateBTTaskNode::GetStaticDescription() const
{
	const FString KeyDescription = TargetToFaceKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Smoothly rotate to face %s key, angle precision : %s"), *KeyDescription, *FString::SanitizeFloat(AnglePrecision));
}

EBTNodeResult::Type UFaceTargetRotateBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetToFaceKey.SelectedKeyName);
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
			RotationInterpolateSpeed);

		Memory->OwnerPawn->SetActorRotation(TargetRotation);
	}
}

bool UFaceTargetRotateBTTaskNode::HasReachedAnglePercision(APawn* QueryPawn, AActor* TargetActor) const
{
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	const FVector OwnerToTargetNormalVector = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalVector);
	const float Angle = UKismetMathLibrary::DegAcos(DotResult);

	return Angle <= AnglePrecision;
}
