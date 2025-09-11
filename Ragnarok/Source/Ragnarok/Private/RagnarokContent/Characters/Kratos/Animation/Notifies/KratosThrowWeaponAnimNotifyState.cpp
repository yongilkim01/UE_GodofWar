// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Animation/Notifies/KratosThrowWeaponAnimNotifyState.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UKratosThrowWeaponAnimNotifyState::UKratosThrowWeaponAnimNotifyState()
{
	ActiveThrowTag = KratosGameplayTags::Kratos_Event_Throw_Weapon;
}

void UKratosThrowWeaponAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (nullptr == MeshComp || nullptr == MeshComp->GetOwner())
	{
		return;
	}

	if (nullptr != ThrowSoundCue)
	{
		UGameplayStatics::SpawnSoundAttached(
			ThrowSoundCue,
			MeshComp,
			FName("Head"),
			FVector::ZeroVector,
			EAttachLocation::KeepRelativeOffset
		);
	}
}

void UKratosThrowWeaponAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

void UKratosThrowWeaponAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (nullptr == MeshComp || nullptr == MeshComp->GetOwner())
	{
		return;
	}

	URagnarokFunctionLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), ActiveThrowTag);

}
