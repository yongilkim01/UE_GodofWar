// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/Components/EnemyCombatComponent.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/CombatSystem/Tags/CombatGameplayTags.h"

#include "AbilitySystemBlueprintLibrary.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	// �̹� �ݸ��� ó������ ���Ϳ� ������ �ȴٸ� �Լ� �ߴ�
	if (true == OverlappedActorArray.Contains(HitActor))
	{
		return;
	}
	OverlappedActorArray.AddUnique(HitActor);

	// TODO: ���� ���� ��� ����
	bool bVaildBlock = false;

	const bool bTargetBlock= false;
	const bool bAttackUnblockable = false;

	// ������ ���� ������ ���� �Ǵ� Ȱ��ȭ�Ǿ� �ְ� �� ������ ������ �� �ִ� ������ ���
	if (true == bTargetBlock && false == bAttackUnblockable)
	{
		// TODO: ������ ��ȿ���� Ȯ��
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetRagnarokOwner();
	EventData.Target = HitActor;
	
	if (true == bVaildBlock)
	{
		// TODO: ���� ó�� ���� ����
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
