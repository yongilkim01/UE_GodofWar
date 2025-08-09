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

	Debug::Print(TEXT("UKratosRollGameplayAbility::ActivateAbility"));

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);

	SetKratosRollingState(true);
	ComputeDodgeDirection();
	BeginSmoothMovement();

	RollMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		RollAnimMontage,
		1.0f,
		NAME_None,
		true,
		1.0f,
		0.0f,
		false);

	if (nullptr != RollMontageTask)
	{
		RollMontageTask->OnCompleted.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageCompleted);
		RollMontageTask->OnBlendOut.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageBlendOut);
		RollMontageTask->OnInterrupted.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageInterrupted);
		RollMontageTask->OnCancelled.AddDynamic(this, &UKratosRollGameplayAbility::OnMontageCancelled);
		RollMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosRollGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	Debug::Print(TEXT("UKratosRollGameplayAbility::EndAbility"));

	EndSmmothMovement();
	SetKratosRollingState(false);

	FTimerDelegate TimerDel;

	TimerDel.BindUFunction(this, FName("OnResetEvasion"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.5f, false);

}

void UKratosRollGameplayAbility::OnMontageCompleted()
{
	Debug::Print(TEXT("UKratosRollGameplayAbility::OnMontageCompleted"));

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosRollGameplayAbility::OnMontageBlendOut()
{
	Debug::Print(TEXT("UKratosRollGameplayAbility::OnMontageBlendOut"));

}

void UKratosRollGameplayAbility::OnMontageInterrupted()
{
	Debug::Print(TEXT("UKratosRollGameplayAbility::OnMontageInterrupted"));

}

void UKratosRollGameplayAbility::OnResetEvasion()
{
	Debug::Print(TEXT("UKratosRollGameplayAbility::OnResetEvasion"));
}

void UKratosRollGameplayAbility::ComputeDodgeDirection()
{
	const float RollSpeed = 1000.0f;

	RollDirection = GetKratosFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();

	FVector ActorForwardVector = GetKratosFromActorInfo()->GetActorForwardVector();
	FVector ActorRightVector = GetKratosFromActorInfo()->GetActorRightVector();

	float AngleRad = FMath::Acos(FVector::DotProduct(ActorForwardVector, RollDirection));
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);
	float DotRight = FVector::DotProduct(ActorRightVector, RollDirection);

	if (0.0f <= AngleDeg && 22.5f >= AngleDeg)
	{
		RollAnimMontage = DodgeForwardAnimMontage;
	}
	else if (22.5f < AngleDeg && 67.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			RollAnimMontage = DodgeRFAnimMontage;
		}
		else
		{
			RollAnimMontage = DodgeLFAnimMontage;
		}
	}
	else if (67.5f < AngleDeg && 112.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			RollAnimMontage = DodgeRightAnimMontage;
		}
		else
		{
			RollAnimMontage = DodgeLeftAnimMontage;
		}
	}
	else if (112.5f < AngleDeg && 157.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			RollAnimMontage = DodgeRBAnimMontage;
		}
		else
		{
			RollAnimMontage = DodgeLBAnimMontage;
		}
	}
	else
	{
		RollAnimMontage = DodgeBackwardAnimMontage;
	}

}

void UKratosRollGameplayAbility::ComputeRollingDirection()
{
	const float RollSpeed = 5000.0f;

	RollDirection = GetKratosFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();

	FVector ActorForwardVector = GetKratosFromActorInfo()->GetActorForwardVector();
	FVector ActorRightVector = GetKratosFromActorInfo()->GetActorRightVector();

	float AngleRad = FMath::Acos(FVector::DotProduct(ActorForwardVector, RollDirection));
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);
	float DotRight = FVector::DotProduct(ActorRightVector, RollDirection);

	if (0.0f <= AngleDeg && 22.5f >= AngleDeg)
	{
		RollAnimMontage = RollingForwardAnimMontage;
	}
	else if (22.5f < AngleDeg && 67.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			RollAnimMontage = RollingRFAnimMontage;
		}
		else
		{
			RollAnimMontage = RollingLFAnimMontage;
		}
	}
	else if (67.5f < AngleDeg && 112.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			RollAnimMontage = RollingRightAnimMontage;
		}
		else
		{
			RollAnimMontage = RollingLeftAnimMontage;
		}
	}
	else if (112.5f < AngleDeg && 157.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			RollAnimMontage = RollingRBAnimMontage;
		}
		else
		{
			RollAnimMontage = RollingLBAnimMontage;
		}
	}
	else
	{
		RollAnimMontage = RollingBackwardAnimMontage;
	}

}

void UKratosRollGameplayAbility::BeginSmoothMovement()
{
	StartLocation = Kratos->GetActorLocation();
	
	float Distance = bEvasion ? RollDistance : DodgeDistance;
	TargetLocation = StartLocation + (RollDirection * Distance);

	ElapsedTime = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(
		MovementTimerHandle,
		this,
		&UKratosRollGameplayAbility::TickSmoothMovement,
		0.016f,
		true
	);
}

void UKratosRollGameplayAbility::TickSmoothMovement()
{
	ElapsedTime += GetWorld()->GetDeltaSeconds();

	float MovementDuration = bEvasion ? RollMovementDuration : DodgeMovementDuration;

	float Alpha = FMath::Clamp(ElapsedTime / MovementDuration, 0.0f, 1.0f);

	if(nullptr != MovementDurationCurve)
	{
		Alpha = MovementDurationCurve->GetFloatValue(Alpha);
	}

	FVector CalLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);

	FHitResult HitResult;
	Kratos->SetActorLocation(CalLocation, true, &HitResult);

	if (Alpha >= 1.0f)
	{
		EndSmmothMovement();
	}
}

void UKratosRollGameplayAbility::EndSmmothMovement()
{
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
}
