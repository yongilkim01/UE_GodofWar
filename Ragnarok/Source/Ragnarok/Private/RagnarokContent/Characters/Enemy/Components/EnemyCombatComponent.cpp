// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/Components/EnemyCombatComponent.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/CombatSystem/Tags/CombatGameplayTags.h"

#include "AbilitySystemBlueprintLibrary.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (true == OverlappedActorArray.Contains(HitActor))
	{
		return;
	}
	OverlappedActorArray.AddUnique(HitActor);

	bool bVaildBlock = false;

	const bool bTargetBlock= false;
	const bool bAttackUnblockable = false;

	if (true == bTargetBlock && false == bAttackUnblockable)
	{
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetRagnarokOwner();
	EventData.Target = HitActor;
	
	if (true == bVaildBlock)
	{
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetRagnarokOwner(),
			CombatGameplayTags::Combat_Event_MeleeHit,
			EventData
		);
	}
}
