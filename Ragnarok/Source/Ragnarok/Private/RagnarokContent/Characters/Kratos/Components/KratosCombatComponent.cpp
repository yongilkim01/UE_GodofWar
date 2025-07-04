// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

AKratosWeapon* UKratosCombatComponent::GetKratosWeaponByTag(FGameplayTag InFindWeaponTag) const
{
    return Cast<AKratosWeapon>(GetCharacterWeaponByTag(InFindWeaponTag));
}

void UKratosCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    Debug::Print(GetRagnarokOwner()->GetActorNameOrLabel() + TEXT(" hit ") + HitActor->GetActorNameOrLabel(), FColor::Green);
}

void UKratosCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
    Debug::Print(GetRagnarokOwner()->GetActorNameOrLabel() + TEXT(" weapon pull ") + InteractedActor->GetActorNameOrLabel(), FColor::Green);
}
