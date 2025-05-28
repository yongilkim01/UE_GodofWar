// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Kratos.h"
#include "RagnarokDebugHelper.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "DataAssets/KratosConfigDataAsset.h"

AKratos::AKratos()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	MainCameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AKratos::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("Start Kratos Beginplay method"));

	if (nullptr != KratosConfig)
	{
		GetCapsuleComponent()->InitCapsuleSize(KratosConfig->CapsuleRadius, KratosConfig->CapsuleHalfHeight);

		SpringArmComponent->TargetArmLength = KratosConfig->TargetArmLength;
		SpringArmComponent->SocketOffset = KratosConfig->SocketOffset;

		GetCharacterMovement()->RotationRate = KratosConfig->CharacterMovementRotationRate;
		GetCharacterMovement()->MaxWalkSpeed = KratosConfig->MaxWalkSpeed;

	}
	else
	{
		Debug::Print(TEXT("Kratos config data assets is nullptr!!"), FColor::Red);
	}
}