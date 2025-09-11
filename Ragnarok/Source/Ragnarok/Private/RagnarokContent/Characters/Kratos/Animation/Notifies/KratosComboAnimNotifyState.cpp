// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Animation/Notifies/KratosComboAnimNotifyState.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"

#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"

UKratosComboAnimNotifyState::UKratosComboAnimNotifyState()
{
	AttackWaitStartEventTag = KratosGameplayTags::Kratos_Event_AttackWait_Start;
	AttackWaitStartEventTag = KratosGameplayTags::Kratos_Event_AttackWait_End;
	ComboIndex = 1;
}

void UKratosComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (nullptr == MeshComp || nullptr == MeshComp->GetOwner())
	{
		return;
	}

	URagnarokFunctionLibrary::SendGameplayEventToActorWithMagnitude(MeshComp->GetOwner(), AttackWaitStartEventTag, ComboIndex);
}

void UKratosComboAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

void UKratosComboAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (nullptr == MeshComp || nullptr == MeshComp->GetOwner())
	{
		return;
	}

	URagnarokFunctionLibrary::SendGameplayEventToActorWithMagnitude(MeshComp->GetOwner(), AttackWaitEndEventTag, ComboIndex);

}
