// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Abilities/AbilityTasks/RKAbilityTask_RotateToCamera.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokGameplayAbility.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

URKAbilityTask_RotateToCamera* URKAbilityTask_RotateToCamera::CreateRotateToCameraTask(URagnarokGameplayAbility* OwnerAbility, FName TaskName)
{
	URKAbilityTask_RotateToCamera* SpawnObj = NewAbilityTask<URKAbilityTask_RotateToCamera>(OwnerAbility, TaskName);
	SpawnObj->bTickingTask = true;
	return SpawnObj;
}

void URKAbilityTask_RotateToCamera::Activate()
{
	Super::Activate();

	//Debug::Print(TEXT("URKAbilityTask_RotateToCamera::Activate called"), FColor::Green);

	RagnarokAbility = Cast<URagnarokGameplayAbility>(Ability);

	if (false == RagnarokAbility.IsValid())
	{
		Debug::Print(TEXT("URKAbilityTask_RotateToCamera::Activate RagnarokAbility is not valid"), FColor::Red);
		EndTask();
		return;
	}

	//Debug::Print(TEXT("URKAbilityTask_RotateToCamera::Activate succeeded"), FColor::Green);
}

void URKAbilityTask_RotateToCamera::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	//Debug::Print(TEXT("URKAbilityTask_RotateToCamera::TickTask called"), FColor::Cyan);

	if (false == RagnarokAbility.IsValid())
	{
		Debug::Print(TEXT("URKAbilityTask_RotateToCamera::TickTask - RagnarokAbility is not valid"), FColor::Red);
		EndTask();
		return;
	}

	RagnarokAbility->RotateOwnerToCameraDirection();
}
