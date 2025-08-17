// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Core/Animation/CharacterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
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

	CalcMovementSpeeds();
}

void UCharacterAnimInstance::CalcMovementSpeeds()
{
	if (nullptr == OwnerCharacter || nullptr == OwnerCharacterMovementComponent)
	{
		return;
	}

	float MaxWalkSpeed = OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed;

	FVector ForwardVector = OwnerCharacter->GetMesh()->GetForwardVector();
	FVector RightVector = OwnerCharacter->GetMesh()->GetRightVector();

	FRotator MeshWorldRotator = OwnerCharacter->GetMesh()->GetComponentRotation();
	FRotator CapsuleWorldRotator = OwnerCharacter->GetCapsuleComponent()->GetComponentRotation();
	
	FRotator PitchRotator(0.0f, MeshWorldRotator.Pitch - CapsuleWorldRotator.Pitch, 0.0f);

	FVector RotateForwardVectorToPitch = PitchRotator.RotateVector(ForwardVector);
	FVector RotateRightVectorToPitch = PitchRotator.RotateVector(RightVector);

	FVector Velocity = OwnerCharacter->GetVelocity();
	float VelocitySize = OwnerCharacter->GetVelocity().Size();
	
	float ForwardDot = FVector::DotProduct(Velocity, RotateForwardVectorToPitch);
	float RightDot = FVector::DotProduct(Velocity, RotateRightVectorToPitch);

	ForwardSpeed = -(RightDot / MaxWalkSpeed) * -1.0f;
	RightSpeed = (ForwardDot / MaxWalkSpeed) * -1.0f;
}
