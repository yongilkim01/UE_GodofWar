// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosLightAttackAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilityFunctionLibrary.h"
#include "RagnarokEngine/Systems/CombatSystem/Tags/CombatGameplayTags.h"

void UKratosLightAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	UseComboCount = CurComboCount;

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		LightAttackMontageMap[CurComboCount],
		1.0f,
		NAME_None,
		false,
		0.0f,
		false
	);

	WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CombatGameplayTags::Combat_Event_MeleeHit,
		nullptr,
		false,
		false
	);

	if (nullptr != WaitEventTask)
	{
		WaitEventTask->EventReceived.AddDynamic(this, &UKratosLightAttackAbility::OnGameplayEventReceived);

		if (true == WaitEventTask->EventReceived.IsBound())
		{
			Debug::Print(TEXT("LightAttack Delegate Binding Sucess"));
		}
		else
		{
			Debug::Print(TEXT("LightAttack Delegate Binding Failed"));
		}

		WaitEventTask->ReadyForActivation();
	}

	if (nullptr != GetKratosFromActorInfo())
	{
		if (GetKratosFromActorInfo()->GetKratosAttackCount() == 0)
		{
			GetKratosFromActorInfo()->GetCharacterMovement()->DisableMovement();
		}

		GetKratosFromActorInfo()->AddKratosAttackCount(1);
	}

	if (CurComboCount == LightAttackMontageMap.Num())
	{
		OnResetAttackComboCount();
	}
	else
	{
		if (CurComboCount + 1 == LightAttackMontageMap.Num())
		{
			URagnarokAbilityFunctionLibrary::AddGameplayTagToActor(
				GetKratosFromActorInfo(), JumpTag);
		}

		CurComboCount++;
	}

	if (nullptr != MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UKratosLightAttackAbility::OnMontageCompleted);
		MontageTask->OnBlendOut.AddDynamic(this, &UKratosLightAttackAbility::OnMontageBlendOut);
		MontageTask->OnInterrupted.AddDynamic(this, &UKratosLightAttackAbility::OnMontageInterrupted);
		MontageTask->OnCancelled.AddDynamic(this, &UKratosLightAttackAbility::OnMontageCancelled);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}


}

void UKratosLightAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (nullptr != GetKratosFromActorInfo())
	{
		GetKratosFromActorInfo()->AddKratosAttackCount(-1);

		if (GetKratosFromActorInfo()->GetKratosAttackCount() <= 0)
		{
			GetKratosFromActorInfo()->SetKratosAttackCount(0);
			GetKratosFromActorInfo()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}

	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("OnResetAttackComboCount"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.3f, false);
}

void UKratosLightAttackAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosLightAttackAbility::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosLightAttackAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosLightAttackAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UKratosLightAttackAbility::OnResetAttackComboCount()
{
	CurComboCount = 1;
	URagnarokAbilityFunctionLibrary::RemoveGameplayTagToActor(GetKratosFromActorInfo(), JumpTag);
	
}

void UKratosLightAttackAbility::OnGameplayEventReceived(FGameplayEventData Payload)
{
	Debug::Print(TEXT("Hitting ") + Payload.Target.GetName() + TEXT(" with light attack (Combo: ") + FString::FromInt(UseComboCount) + TEXT(")"), FColor::Cyan);
}
