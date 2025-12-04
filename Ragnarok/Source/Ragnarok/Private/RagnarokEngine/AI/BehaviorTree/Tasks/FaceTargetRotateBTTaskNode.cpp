// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/AI/BehaviorTree/Tasks/FaceTargetRotateBTTaskNode.h"
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

	// �����Ϳ��� ������ Ű �̸��� �������� Ű�� ����
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
	UObject* TargetActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetActorObject);

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	FFaceTargetRotateTaskMemory* InstanceMemory = CastInstanceNodeMemory<FFaceTargetRotateTaskMemory>(NodeMemory);

	InstanceMemory->OwnerPawn = OwnerPawn;
	InstanceMemory->TargetActor = TargetActor;

	if (false == InstanceMemory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	if (true == HasReachedTargetActor(OwnerPawn, TargetActor))
	{
		InstanceMemory->Reset();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UFaceTargetRotateBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FFaceTargetRotateTaskMemory* InstanceMemory = CastInstanceNodeMemory<FFaceTargetRotateTaskMemory>(NodeMemory);

	if (false == InstanceMemory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (true == HasReachedTargetActor(InstanceMemory->OwnerPawn.Get(), InstanceMemory->TargetActor.Get()))
	{
		InstanceMemory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
			InstanceMemory->OwnerPawn->GetActorLocation(), InstanceMemory->TargetActor->GetActorLocation());

		const FRotator TargetRotation = FMath::RInterpTo(
			InstanceMemory->OwnerPawn->GetActorRotation(), 
			LookAtRotation, DeltaSeconds, 
			RotationInterpSpeed);

		InstanceMemory->OwnerPawn->SetActorRotation(TargetRotation);
	}
}

bool UFaceTargetRotateBTTaskNode::HasReachedTargetActor(APawn* OwnerPawn, AActor* TargetActor) const
{
	const FVector OwnerForwardVector = OwnerPawn->GetActorForwardVector();
	const FVector OwnerToTargetNormalVector = (TargetActor->GetActorLocation() - OwnerPawn->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(OwnerForwardVector, OwnerToTargetNormalVector);
	const float Angle = UKismetMathLibrary::DegAcos(DotResult);

	return Angle <= AngleThreshold;
}
