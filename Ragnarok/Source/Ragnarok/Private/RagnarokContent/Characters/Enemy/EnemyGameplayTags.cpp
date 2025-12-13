// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/EnemyGameplayTags.h"

namespace EnemyGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged, "Enemy.Ability.Ranged");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing, "Enemy.Status.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Knockback, "Enemy.Status.Knockback");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Launch, "Enemy.Status.Launch");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Slamdown, "Enemy.Status.Slamdown");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnderAttack, "Enemy.Status.UnderAttack");
}