// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/CombatSystem/Notifies/WeaponCollisionAnimNotifyState.h"
#include "RagnarokEngine/CombatSystem/CombatFunctionLibrary.h"
#include "RagnarokEngine/CombatSystem/CombatComponent.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

void UWeaponCollisionAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UCombatComponent* CombatComponent = UCombatFunctionLibrary::GetCombatComponentFromActor(MeshComp->GetOwner());

	if (nullptr != CombatComponent)
	{
		CombatComponent->ToggleWeaponCollision(true, ToggleWeaponType);
	}
}

void UWeaponCollisionAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

void UWeaponCollisionAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	UCombatComponent* CombatComponent = UCombatFunctionLibrary::GetCombatComponentFromActor(MeshComp->GetOwner());

	if (nullptr != CombatComponent)
	{
		CombatComponent->ToggleWeaponCollision(false, ToggleWeaponType);
	}
}
