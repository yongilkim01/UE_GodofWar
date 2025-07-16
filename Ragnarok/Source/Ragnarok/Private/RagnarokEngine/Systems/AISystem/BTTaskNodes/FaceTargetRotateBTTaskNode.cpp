// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/AISystem/BTTaskNodes/FaceTargetRotateBTTaskNode.h"

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
