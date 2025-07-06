// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"

#include "NativeGameplayTags.h"

namespace RagnarokGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look")

	UE_DEFINE_GAMEPLAY_TAG(Global_SetByCaller_BaseDamage, "Global.SetByCaller.BaseDamage")
	UE_DEFINE_GAMEPLAY_TAG(Global_Ability_HitReact, "Global.Ability.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(Global_Event_HitReact, "Global.Event.HitReact")
}