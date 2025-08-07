// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosRollGameplayAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameplayTagContainer.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UKratosRollGameplayAbility::UKratosRollGameplayAbility()
{
	//BlockAbilitiesWithTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Kratos.Ability")));
}

void UKratosRollGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 0.05f);

	if (nullptr != DelayTask)
	{
		DelayTask->OnFinish.AddDynamic(this, &UKratosRollGameplayAbility::OnDelayFinished);
		DelayTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

}

void UKratosRollGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	CurrentActorInfo->OwnerActor.Get()->SetActorRotation(PrevRotator);
	SetKratosRollingState(false);
	GetKratosFromActorInfo()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("OnResetEvasion"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.5f, false);

}

void UKratosRollGameplayAbility::OnDelayFinished()
{
	PrevRotator = CurrentActorInfo->OwnerActor.Get()->GetActorRotation();
	SetKratosRollingState(true);
	GetKratosFromActorInfo()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	if (bEvasion)
	{
		ComputeRollingDirection();
		bEvasion = false;
	}
	else
	{
		ComputeDodgeDirection();
		bEvasion = true;

	}

	if (nullptr != AbilityAnimMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			AbilityAnimMontage,
			1.0f,
			NAME_None,
			true,
			1.0f,
			0.0f,
			false);

		if (nullptr != MontageTask)
		{
			MontageTask->OnCompleted.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageCompleted);
			MontageTask->OnBlendOut.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageBlendOut);
			MontageTask->OnInterrupted.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageInterrupted);
			MontageTask->OnCancelled.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageCancelled);
			MontageTask->ReadyForActivation();
		}
		else
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosRollGameplayAbility::OnResetEvasion()
{
	bEvasion = false;
}

void UKratosRollGameplayAbility::ComputeDodgeDirection()
{
	const float RollSpeed = 1000.0f;

	RollingDirection = GetKratosFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();

	GetKratosFromActorInfo()->LaunchCharacter(RollingDirection * RollSpeed, true, true);

	UMotionWarpingComponent* MortionWarpingComponent = GetKratosFromActorInfo()->GetMotionWarpingComponent();

	if (nullptr != MortionWarpingComponent)
	{
		const FVector StartLocation = GetKratosFromActorInfo()->GetActorLocation();
		const FVector TargetLocation = StartLocation + (RollingDirection * RollDistance);
		MortionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
			WarpTargetName,
			TargetLocation,
			FRotator::ZeroRotator
		);
	}

	FVector ActorForwardVector = GetKratosFromActorInfo()->GetActorForwardVector();
	FVector ActorRightVector = GetKratosFromActorInfo()->GetActorRightVector();

	float AngleRad = FMath::Acos(FVector::DotProduct(ActorForwardVector, RollingDirection));
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);
	float DotRight = FVector::DotProduct(ActorRightVector, RollingDirection);

	if (0.0f <= AngleDeg && 22.5f >= AngleDeg)
	{
		AbilityAnimMontage = DodgeForwardAnimMontage;
	}
	else if (22.5f < AngleDeg, 67.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			AbilityAnimMontage = DodgeRFAnimMontage;
		}
		else
		{
			AbilityAnimMontage = DodgeLFAnimMontage;
		}
	}
	else if (67.5f < AngleDeg && 112.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			AbilityAnimMontage = DodgeRightAnimMontage;
		}
		else
		{
			AbilityAnimMontage = DodgeLeftAnimMontage;
		}
	}
	else if (112.5f < AngleDeg, 157.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			AbilityAnimMontage = DodgeRBAnimMontage;
		}
		else
		{
			AbilityAnimMontage = DodgeLBAnimMontage;
		}
	}
	else
	{
		AbilityAnimMontage = DodgeBackwardAnimMontage;
	}

}

void UKratosRollGameplayAbility::ComputeRollingDirection()
{
	const float RollSpeed = 5000.0f;

	RollingDirection = GetKratosFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();
	UMotionWarpingComponent* MortionWarpingComponent = GetKratosFromActorInfo()->GetMotionWarpingComponent();
	GetKratosFromActorInfo()->LaunchCharacter(RollingDirection * RollSpeed, true, true);

	/*if (nullptr != MortionWarpingComponent)
	{
		const FVector StartLocation = GetKratosFromActorInfo()->GetActorLocation();
		const FVector TargetLocation = StartLocation + (RollingDirection * RollDistance);
		MortionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
			WarpTargetName,
			FVector::ZeroVector,
			RollingDirection.ToOrientationRotator()
		);
	}*/

	FVector ActorForwardVector = GetKratosFromActorInfo()->GetActorForwardVector();
	FVector ActorRightVector = GetKratosFromActorInfo()->GetActorRightVector();

	float AngleRad = FMath::Acos(FVector::DotProduct(ActorForwardVector, RollingDirection));
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);
	float DotRight = FVector::DotProduct(ActorRightVector, RollingDirection);

	//AbilityAnimMontage = RollingForwardAnimMontage;


	if (0.0f <= AngleDeg && 22.5f >= AngleDeg)
	{
		AbilityAnimMontage = RollingForwardAnimMontage;
	}
	else if (22.5f < AngleDeg, 67.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			AbilityAnimMontage = RollingRFAnimMontage;
		}
		else
		{
			AbilityAnimMontage = RollingLFAnimMontage;
		}
	}
	else if (67.5f < AngleDeg && 112.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			AbilityAnimMontage = RollingRightAnimMontage;
		}
		else
		{
			AbilityAnimMontage = RollingLeftAnimMontage;
		}
	}
	else if (112.5f < AngleDeg, 157.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			AbilityAnimMontage = RollingRBAnimMontage;
		}
		else
		{
			AbilityAnimMontage = RollingLBAnimMontage;
		}
	}
	else
	{
		AbilityAnimMontage = RollingBackwardAnimMontage;
	}

}
