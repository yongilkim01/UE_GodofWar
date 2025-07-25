// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosRollGameplayAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

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
	GetKratosFromActorInfo()->SetIsRolling(false);
	GetKratosFromActorInfo()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}

void UKratosRollGameplayAbility::OnDelayFinished()
{
	PrevRotator = CurrentActorInfo->OwnerActor.Get()->GetActorRotation();
	GetKratosFromActorInfo()->SetIsRolling(true);
	GetKratosFromActorInfo()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	ComputeRollDirectionAndDistance();

	// 1. 구르기 방향을 결정합니다.
	const FVector RollDirection = GetKratosFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();

	// 2. 구르기 속도를 설정합니다. (이 값을 조절해 세기 조절)
	const float RollSpeed = 1500.0f;

	// 3. LaunchCharacter를 호출하여 캐릭터에게 속도를 부여합니다.
	GetKratosFromActorInfo()->LaunchCharacter(RollDirection * RollSpeed, true, true);

	// 캐릭터가 입력 방향을 즉시 바라보게 합니다.
	//Kratos->SetActorRotation(RollDirection.ToOrientationRotator());

	if (nullptr != RollingAnimMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			RollingAnimMontage,
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

void UKratosRollGameplayAbility::ComputeRollDirectionAndDistance()
{
	RollingDirection = GetKratosFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();
	UMotionWarpingComponent* MortionWarpingComponent = GetKratosFromActorInfo()->GetMotionWarpingComponent();

	Debug::Print(FString::Printf(TEXT("Last Input Vector : %s"), *RollingDirection.ToString()), FColor::Yellow);
	Debug::Print(FString::Printf(TEXT("Forward Vector : %s"), *GetKratosFromActorInfo()->GetActorForwardVector().ToString()), FColor::Yellow);

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
		RollingAnimMontage = RollingForwardAnimMontage;
	}
	else if (22.5f < AngleDeg, 67.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			RollingAnimMontage = RollingRFAnimMontage;
		}
		else
		{
			RollingAnimMontage = RollingLFAnimMontage;
		}
	}
	else if (67.5f < AngleDeg && 112.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			RollingAnimMontage = RollingRightAnimMontage;
		}
		else
		{
			RollingAnimMontage = RollingLeftAnimMontage;
		}
	}
	else if (112.5f < AngleDeg, 157.5f >= AngleDeg)
	{
		if (DotRight >= 0.0f)
		{
			RollingAnimMontage = RollingRBAnimMontage;
		}
		else
		{
			RollingAnimMontage = RollingLBAnimMontage;
		}
	}
	else
	{
		RollingAnimMontage = RollingBackwardAnimMontage;
	}

}
