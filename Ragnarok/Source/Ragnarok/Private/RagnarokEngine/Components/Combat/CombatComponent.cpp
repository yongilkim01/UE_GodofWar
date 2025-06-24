// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Components/Combat/CombatComponent.h"

#include "RagnarokEngine/GameItem/Weapon/RagnarokWeapon.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

void UCombatComponent::RegisterSpawnWeapon(FGameplayTag InRegisterWeaponTag, ARagnarokWeapon* InRegisterWeapon, bool bEquipWeapon)
{
	if (true == CharacterWeaponMap.Contains(InRegisterWeaponTag))
	{
		Debug::Print(TEXT("register weapon tag is already register"), FColor::Red, true);
	}
	
	CharacterWeaponMap.Emplace(InRegisterWeaponTag, InRegisterWeapon);

	if (true == bEquipWeapon)
	{
		CurrentEquippedWeaponTag = InRegisterWeaponTag;
	}

	const FString WeaponString = FString::Printf(TEXT("Weapon name : %s is registered %s"), *InRegisterWeapon->GetName(), *InRegisterWeaponTag.ToString());
	Debug::Print(WeaponString);
}

ARagnarokWeapon* UCombatComponent::GetCharacterWeaponByTag(FGameplayTag InFindWeaponTag) const
{
	if (true == CharacterWeaponMap.Contains(InFindWeaponTag))
	{
		ARagnarokWeapon* const* FindWeapon = CharacterWeaponMap.Find(InFindWeaponTag);

		if (nullptr != FindWeapon)
		{
			return *FindWeapon;
		}
	}
	return nullptr;
}

ARagnarokWeapon* UCombatComponent::GetCurrentEquippedWeapon() const
{
	if (false == CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterWeaponByTag(CurrentEquippedWeaponTag);
}
