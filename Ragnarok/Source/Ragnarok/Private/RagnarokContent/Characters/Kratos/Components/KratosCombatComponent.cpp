// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"

AKratosWeapon* UKratosCombatComponent::GetKratosWeaponByTag(FGameplayTag InFindWeaponTag) const
{
    return Cast<AKratosWeapon>(GetCharacterWeaponByTag(InFindWeaponTag));
}
