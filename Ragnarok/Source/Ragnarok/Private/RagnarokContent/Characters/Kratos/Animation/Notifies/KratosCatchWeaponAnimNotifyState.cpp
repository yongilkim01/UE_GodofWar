// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Animation/Notifies/KratosCatchWeaponAnimNotifyState.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"

UKratosCatchWeaponAnimNotifyState::UKratosCatchWeaponAnimNotifyState()
{
}

void UKratosCatchWeaponAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (nullptr == MeshComp || nullptr == MeshComp->GetOwner())
	{
		return;
	}

	AKratos* OwnerKratos = Cast<AKratos>(MeshComp->GetOwner());

	if (nullptr == OwnerKratos)
	{
		return;
	}

	UKratosCombatComponent* KratosCombatComponent = OwnerKratos->GetKratosCombatComponent();

	if (nullptr == KratosCombatComponent)
	{
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (nullptr == AnimInstance)
	{
		return;
	}

	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (nullptr == CurrentMontage)
	{
		return;
	}

	AnimInstance->Montage_SetPlayRate(CurrentMontage, 2.0f);
}

void UKratosCatchWeaponAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

void UKratosCatchWeaponAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (nullptr == MeshComp || nullptr == MeshComp->GetOwner())
	{
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (nullptr == AnimInstance)
	{
		return;
	}

	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (nullptr == CurrentMontage)
	{
		return;
	}

	AnimInstance->Montage_SetPlayRate(CurrentMontage, 1.0f);
}
