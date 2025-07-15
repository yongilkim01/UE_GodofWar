// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Core/Animation/CharacterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"

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

	LocomotionDirection =  UKismetAnimationLibrary::CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation());

}