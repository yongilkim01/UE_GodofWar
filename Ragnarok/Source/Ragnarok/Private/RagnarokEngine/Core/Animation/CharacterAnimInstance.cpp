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
	
	float PitchValue = MeshWorldRotator.Pitch - CapsuleWorldRotator.Pitch;

	FVector RotateForwardVectorToPitch = ForwardVector.RotateAngleAxis(PitchValue, FVector::UpVector);
	FVector RotateRightVectorToPitch = RightVector.RotateAngleAxis(PitchValue, FVector::UpVector);

	FVector NormalVelocity = OwnerCharacter->GetVelocity().GetSafeNormal();
	float VelocitySize = OwnerCharacter->GetVelocity().Size();
	
	float ForwardDot = FVector::DotProduct(NormalVelocity, RotateForwardVectorToPitch);
	float RightDot = FVector::DotProduct(NormalVelocity, RotateRightVectorToPitch);

	ForwardSpeed = FMath::Clamp((ForwardDot * VelocitySize) / MaxWalkSpeed, -1.0f, 1.0f) * MaxWalkSpeed;
	RightSpeed = FMath::Clamp((RightDot * VelocitySize) / MaxWalkSpeed, -1.0f, 1.0f) * MaxWalkSpeed;
}
