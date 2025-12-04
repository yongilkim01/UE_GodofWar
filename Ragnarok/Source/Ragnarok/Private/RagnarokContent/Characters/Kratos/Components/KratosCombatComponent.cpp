// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Core/CombatSystem/Tags/CombatGameplayTags.h"

#include "AbilitySystemBlueprintLibrary.h"

AKratosWeapon* UKratosCombatComponent::GetKratosWeaponByTag(FGameplayTag InFindWeaponTag) const
{
    return Cast<AKratosWeapon>(GetCharacterWeaponByTag(InFindWeaponTag));
}

AKratosWeapon* UKratosCombatComponent::GetKratosEquippedWeapopn() const
{
    return GetKratosWeaponByTag(CurrentEquippedWeaponTag);

}

float UKratosCombatComponent::GetKratosEquippedWeaponDamageAtLevel(float Level) const
{
    return GetKratosEquippedWeapopn()->WeaponData.WeaponDamage.GetValueAtLevel(Level);
}

void UKratosCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    if (true == OverlappedActorArray.Contains(HitActor))
    {
        return;
    }

    OverlappedActorArray.AddUnique(HitActor);
    
    FGameplayEventData Data;
    Data.Instigator = GetRagnarokOwner();
    Data.Target = HitActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetRagnarokOwner(),
        CombatGameplayTags::Combat_Event_MeleeHit,
        Data
    );

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetRagnarokOwner(),
        KratosGameplayTags::Kratos_Event_HitPause,
        FGameplayEventData()
    );
  
}

void UKratosCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetRagnarokOwner(),
        KratosGameplayTags::Kratos_Event_HitPause,
        FGameplayEventData()
    );
}
