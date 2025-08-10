// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"

#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

void UKratosGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Kratos = GetKratosFromActorInfo();

	if (nullptr == Kratos)
	{
		Debug::Print(TEXT("UKratosGameplayAbility::Kratos is nullptr"), FColor::Red);
	}
}

AKratos* UKratosGameplayAbility::GetKratosFromActorInfo()
{
	if (false == Kratos.IsValid())
	{
		Kratos = Cast<AKratos>(CurrentActorInfo->AvatarActor);;
	}

	return Kratos.Get();
}

AKratosController* UKratosGameplayAbility::GetKratosControllerFromActorInfo()
{
	if (false == KratosController.IsValid())
	{
		APawn* Pawn = Cast<APawn>(CurrentActorInfo->AvatarActor);

		if (nullptr != Pawn)
		{
			KratosController = Cast<AKratosController>(Pawn->GetController());
		}
	}

	return KratosController.Get();
}

UKratosCombatComponent* UKratosGameplayAbility::GetKratosCombatComponent()
{
	return GetKratosFromActorInfo()->GetKratosCombatComponent();
}

FGameplayEffectSpecHandle UKratosGameplayAbility::CreateKratosDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float Damage, FGameplayTag AttackTypeTag, int32 ComboCount)
{
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetASCFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetASCFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(RagnarokGameplayTags::Global_SetByCaller_BaseDamage, Damage);

	if (true == AttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(AttackTypeTag, ComboCount);
	}

	return EffectSpecHandle;
}

void UKratosGameplayAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosGameplayAbility::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosGameplayAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosGameplayAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosGameplayAbility::SetKratosAttackingState(bool bAttacking)
{
	Kratos->bAttacking = bAttacking;
}

void UKratosGameplayAbility::SetKratosRollingState(bool bRolling)
{
	Kratos->bRolling = bRolling;
}

bool UKratosGameplayAbility::IsKratosRunning()
{
	return Kratos->bRunning;
}
