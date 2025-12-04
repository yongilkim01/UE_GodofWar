// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Core/CombatSystem/CombatComponent.h"
#include "RagnarokContent/Items/Weapons/RagnarokWeapon.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "Components/BoxComponent.h"

void UCombatComponent::RegisterSpawnWeapon(FGameplayTag InRegisterWeaponTag, ARagnarokWeapon* InRegisterWeapon, bool bEquipWeapon)
{
	if (true == CharacterWeaponMap.Contains(InRegisterWeaponTag))
	{
		Debug::Print(TEXT("register weapon tag is already register"), FColor::Red, true);
	}
	
	CharacterWeaponMap.Emplace(InRegisterWeaponTag, InRegisterWeapon);

	InRegisterWeapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InRegisterWeapon->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	if (true == bEquipWeapon)
	{
		CurrentEquippedWeaponTag = InRegisterWeaponTag;
	}

	const FString WeaponString = FString::Printf(TEXT("Weapon name : %s is registered %s"), *InRegisterWeapon->GetName(), *InRegisterWeaponTag.ToString());
	if(true == bShowDebugMsg) Debug::Print(WeaponString);
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
#if WITH_EDITOR
	if (true == GIsEditor && nullptr != GetWorld() && false == GetWorld()->IsGameWorld())
	{
		return nullptr;
	}
#endif

	if (FGameplayTag::EmptyTag == CurrentEquippedWeaponTag)
	{
		return nullptr;
	}

	return GetCharacterWeaponByTag(CurrentEquippedWeaponTag);
}

void UCombatComponent::ToggleWeaponCollision(bool bCollisionEnable, EToggleDamageType ToggleDamageType)
{
	if (EToggleDamageType::ETD_CurrentEquippedWeapon == ToggleDamageType)
	{
		ARagnarokWeapon* CurrentWeapon = GetCurrentEquippedWeapon();

		if (nullptr == CurrentWeapon)
		{
			return;
		}

		if (true == bCollisionEnable)
		{
			CurrentWeapon->GetWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			CurrentWeapon->GetWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			OverlappedActorArray.Empty();
		}
	}
}

void UCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}
