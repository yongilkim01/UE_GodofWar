// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimInstances/CharacterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/RagnarokCharacter.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	OwnerCharacter = Cast<ARagnarokCharacter>(TryGetPawnOwner());

	if (nullptr != OwnerCharacter)
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (nullptr == OwnerCharacter || nullptr == OwnerCharacterMovementComponent)
	{
		return;
	}

	GroundSpeed = OwnerCharacter->GetVelocity().Size2D();

	bAcceleration = OwnerCharacterMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.0f;
}