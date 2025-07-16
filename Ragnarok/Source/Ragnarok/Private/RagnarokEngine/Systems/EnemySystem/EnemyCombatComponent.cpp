// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/EnemySystem/EnemyCombatComponent.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/CombatSystem/Tags/CombatGameplayTags.h"

#include "AbilitySystemBlueprintLibrary.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	// 이미 콜리전 처리가된 액터에 포함이 된다면 함수 중단
	if (true == OverlappedActorArray.Contains(HitActor))
	{
		return;
	}
	OverlappedActorArray.AddUnique(HitActor);

	// TODO: 공격 블록 기능 구현
	bool bVaildBlock = false;

	const bool bTargetBlock= false;
	const bool bAttackUnblockable = false;

	// 상대방의 공격 블록이 실행 또는 활성화되어 있고 내 공격이 블록할 수 있는 공격인 경우
	if (true == bTargetBlock && false == bAttackUnblockable)
	{
		// TODO: 블록이 유효한지 확인
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetRagnarokOwner();
	EventData.Target = HitActor;
	
	if (true == bVaildBlock)
	{
		// TODO: 블록 처리 성공 구현
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
