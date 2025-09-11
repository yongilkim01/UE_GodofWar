// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosThrowWeaponAbility.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/Animation/KratosLinkedAnimLayer.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"

#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"

UKratosThrowWeaponAbility::UKratosThrowWeaponAbility()
{
	bShowDebug = true;
}

void UKratosThrowWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::ActivateAbility"));

	CurWeapon = Cast<AKratosWeapon>(Kratos->GetKratosCombatComponent()->GetCurrentEquippedWeapon());

	if (nullptr == CurWeapon)
	{
		Debug::Print(TEXT("UKratosThrowWeaponAbility::CurWeapon is nullptr"), FColor::Red);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	Kratos->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	ThrowWaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		KratosGameplayTags::Kratos_Event_Throw_Weapon,
		nullptr,
		false,
		false
	);

	if (nullptr != ThrowWaitEventTask)
	{
		ThrowWaitEventTask->EventReceived.AddDynamic(this, &UKratosThrowWeaponAbility::OnThrowEventReceived);
		ThrowWaitEventTask->ReadyForActivation();
	}

	PlayThrowAnimMontage();
}

void UKratosThrowWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::EndAbility"));

	Kratos->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//ThrowWeapon();
}

void UKratosThrowWeaponAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::InputPressed"));
}

void UKratosThrowWeaponAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::InputReleased"));
}

bool UKratosThrowWeaponAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (false == Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AKratos* KratosCharacter = Cast<AKratos>(ActorInfo->AvatarActor.Get());

	if (nullptr != KratosCharacter)
	{
		if (false == URagnarokFunctionLibrary::HasActorGameplayTag(ActorInfo->AvatarActor.Get(), KratosGameplayTags::Kratos_Status_Aiming))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		Debug::Print(TEXT("Kratos is nullptr"), FColor::Red);
		return false;
	}
}

void UKratosThrowWeaponAbility::ThrowWeapon()
{
	if (true == bShowDebug) Debug::Print(TEXT("UKratosThrowWeaponAbility::ThrowWeapon"));

	if (nullptr == CurWeapon)
	{
		return;
	}

	USkeletalMeshComponent* ParentMesh = Kratos->FindComponentByClass<USkeletalMeshComponent>();

	if (nullptr != ParentMesh)
	{
		FDetachmentTransformRules DetachRules(
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld,
			true
		);

		CurWeapon->DetachFromActor(DetachRules);
	}
	//ParentMesh->UnlinkAnimClassLayers(CurWeapon->WeaponData.WeaponAnimLayer.Get());
	//ULocalPlayer* LocalPlayer = GetKratosControllerFromActorInfo()->GetLocalPlayer();
	//UEnhancedInputLocalPlayerSubsystem* InputSubsystem
	//	= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	//InputSubsystem->RemoveMappingContext(CurWeapon->WeaponData.InputMappingContext);
	//GetASCFromActorInfo()->RemoveWeaponAbilities(CurWeapon->GetGrantedAbilitySpecHandleArray());
	//GetCombatComponentFromActorInfo()->CurrentEquippedWeaponTag = FGameplayTag::EmptyTag;

	//FVector StartLocation = CurWeapon->GetActorLocation();
	//FVector TargetLocation = FVector::ZeroVector;
	//
	//Kratos->GetAimingTargetLocation(StartLocation, TargetLocation);

	//CurWeapon->ThrowWeaponToTarget(StartLocation, TargetLocation);

	Kratos->ThrowWeapon(CurWeapon);
}

void UKratosThrowWeaponAbility::PlayThrowAnimMontage()
{
	if (nullptr == ThrowAnimMontage)
	{
		Debug::Print(TEXT("UKratosThrowWeaponAbility::ThrowAnimMontage is nullptr"), FColor::Red);
		return;
	}

	PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		ThrowAnimMontage,
		1.0f,
		NAME_None,
		true,
		1.0f,
		0.037f,
		false
	);

	if (nullptr != PlayMontageTask)
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &UKratosThrowWeaponAbility::OnMontageCompleted);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &UKratosThrowWeaponAbility::OnMontageBlendOut);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UKratosThrowWeaponAbility::OnMontageInterrupted);
		PlayMontageTask->OnCancelled.AddDynamic(this, &UKratosThrowWeaponAbility::OnMontageCancelled);
		PlayMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UKratosThrowWeaponAbility::OnThrowEventReceived(FGameplayEventData Payload)
{
	ThrowWeapon();
}
