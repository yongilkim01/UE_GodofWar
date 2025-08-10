// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosRollGameplayAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"

#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameplayTagContainer.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


UKratosRollGameplayAbility::UKratosRollGameplayAbility()
{
	bShowDebug = true;
}

void UKratosRollGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosRollGameplayAbility::ActivateAbility method is called"));

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	CurRollState = ERagnarokRollState::ERRS_Dodge;
	SetKratosRollingState(true);
	CalcAndPlayAnimMontage();
	BeginSmoothMovement();
}

void UKratosRollGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (true == bShowDebug) Debug::Print(TEXT("UKratosRollGameplayAbility::EndAbility method is called"));

	EndSmmothMovement();
	SetKratosRollingState(false);

	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("OnResetEvasion"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.5f, false);

}

void UKratosRollGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(true == bShowDebug) Debug::Print(TEXT("UKratosRollGameplayAbility::InputPressed method is called"));
	
	if (ERagnarokRollState::ERRS_Dodge != CurRollState)
	{
		return;
	}

	if (nullptr != RollMontageTask)
	{
		RollMontageTask->EndTask();
		RollMontageTask = nullptr;
	}

	CurRollState = ERagnarokRollState::ERRS_Roll;
	CalcAndPlayAnimMontage();
	BeginSmoothMovement();

}

void UKratosRollGameplayAbility::CalcAndPlayAnimMontage()
{
	CalcAnimMontage();
	PlayRollAnimMontage();
}

void UKratosRollGameplayAbility::CalcAnimMontage()
{
	const FVector2D InputVector = GetKratosFromActorInfo()->GetCachedMovementInputVector();

	if (false == GetKratosFromActorInfo()->GetCachedMovementInputVector().IsNearlyZero())
	{
		const FRotator YawRotation(0.0f, GetKratosControllerFromActorInfo()->GetControlRotation().Yaw, 0.0f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		FVector WorldDirection = (ForwardDirection * InputVector.Y) + (RightDirection * InputVector.X);
		RollDirection = WorldDirection.GetSafeNormal();
	}
	else
	{
		RollDirection = Kratos->GetActorForwardVector();
	}

	FVector ActorForwardVector = GetKratosFromActorInfo()->GetActorForwardVector();
	FVector ActorRightVector = GetKratosFromActorInfo()->GetActorRightVector();

	float AngleRad = FMath::Acos(FVector::DotProduct(ActorForwardVector, RollDirection));
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);
	float DotRight = FVector::DotProduct(ActorRightVector, RollDirection);

	ERagnarokDirection CalcDirection = ERagnarokDirection::ERD_None;

	if (0.0f <= AngleDeg && 22.5f >= AngleDeg)
	{
		CalcDirection = ERagnarokDirection::ERD_Forward;
	}
	else if (22.5f < AngleDeg && 67.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			CalcDirection = ERagnarokDirection::ERD_ForwardRight;
		}
		else
		{
			CalcDirection = ERagnarokDirection::ERD_ForwardLeft;
		}
	}
	else if (67.5f < AngleDeg && 112.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			CalcDirection = ERagnarokDirection::ERD_Right;
		}
		else
		{
			CalcDirection = ERagnarokDirection::ERD_Left;
		}
	}
	else if (112.5f < AngleDeg && 157.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			CalcDirection = ERagnarokDirection::ERD_BackwardRight;
		}
		else
		{
			CalcDirection = ERagnarokDirection::ERD_BackwardLeft;
		}
	}
	else
	{
		CalcDirection = ERagnarokDirection::ERD_Backward;
	}

	if (ERagnarokRollState::ERRS_Dodge == CurRollState)
	{
		if (false == DodgeAnimMontageMap.Contains(CalcDirection))
		{
			Debug::Print(TEXT("UKratosRollGameplayAbility::DodgeAnimMontageMap have not CalcDirection Anim Montage"), FColor::Red);
			return;
		}

		RollAnimMontage = DodgeAnimMontageMap[CalcDirection];
	}
	else if (ERagnarokRollState::ERRS_Roll == CurRollState)
	{
		if (false == RollAnimMontageMap.Contains(CalcDirection))
		{
			Debug::Print(TEXT("UKratosRollGameplayAbility::RollAnimMontageMap have not CalcDirection Anim Montage"), FColor::Red);
			return;
		}

		RollAnimMontage = RollAnimMontageMap[CalcDirection];
	}
	else
	{
		Debug::Print(TEXT("UKratosRollGameplayAbility::CurRollState is None"), FColor::Red);
		return;
	}
}

void UKratosRollGameplayAbility::PlayRollAnimMontage()
{
	if (nullptr == RollAnimMontage)
	{
		Debug::Print(TEXT("UKratosRollGameplayAbility::RollAnimMontage is nullptr"), FColor::Red);
		return;
	}

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


void UKratosRollGameplayAbility::OnMontageCompleted()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosRollGameplayAbility::OnMontageCompleted method is called"));

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UKratosRollGameplayAbility::OnMontageBlendOut()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosRollGameplayAbility::OnMontageBlendOut method is called"));

}

void UKratosRollGameplayAbility::OnMontageInterrupted()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosRollGameplayAbility::OnMontageInterrupted method is called"));

}

void UKratosRollGameplayAbility::OnResetEvasion()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosRollGameplayAbility::OnResetEvasion method is called"));
}

void UKratosRollGameplayAbility::BeginSmoothMovement()
{
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);

	StartLocation = Kratos->GetActorLocation();
	
	float Distance = 0.0f;

	switch (CurRollState)
	{
	case ERagnarokRollState::ERRS_Dodge:
		Distance = DodgeDistance;
		break;
	case ERagnarokRollState::ERRS_Roll:
		Distance = RollDistance;
		break;
	default:
		break;
	}

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

	float MovementDuration = 0.0f;

	switch (CurRollState)
	{
	case ERagnarokRollState::ERRS_Dodge:
		MovementDuration = DodgeMovementDuration;
		break;
	case ERagnarokRollState::ERRS_Roll:
		MovementDuration = RollMovementDuration;
		break;
	default:
		break;
	}

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

