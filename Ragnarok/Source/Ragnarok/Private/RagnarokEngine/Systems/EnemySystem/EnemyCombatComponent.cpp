// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/EnemySystem/EnemyCombatComponent.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (nullptr != HitActor)
	{
		Debug::Print(GetRagnarokOwner()->GetActorNameOrLabel() + TEXT(" is hitting ") + HitActor->GetActorNameOrLabel());
	}
}
