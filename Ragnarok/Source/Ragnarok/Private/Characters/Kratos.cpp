// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Kratos.h"
#include "RagnarokDebugHelper.h"

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "DataAssets/KratosConfigDataAsset.h"
#include "Data/Kratos/CharacterPrimaryAssetKratos.h"

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

	CharacterPDAId = FPrimaryAssetId(TEXT("CharacterPrimaryAssetKratos"), TEXT("PDA_Kratos"));
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

		UAssetManager::Get().LoadPrimaryAsset
		(
			CharacterPDAId,
			{},
			FStreamableDelegate::CreateUObject(this, &AKratos::LoadKratosConfigData)
		);

	}
	else
	{
		Debug::Print(TEXT("Kratos config data assets is nullptr!!"), FColor::Red);
	}
}

void AKratos::LoadKratosConfigData()
{
	UObject* AssetObject = UAssetManager::Get().GetPrimaryAssetObject(CharacterPDAId);
	CharacterPDA = Cast<UCharacterPrimaryAssetKratos>(AssetObject);

	if (nullptr != CharacterPDA && true == CharacterPDA->KratosSkeletalMesh.IsValid())
	{
		GetMesh()->SetSkeletalMesh(CharacterPDA->KratosSkeletalMesh.Get());
	}
	else
	{
		Debug::Print(TEXT("Kratos config primary data assets is nullptr!!"), FColor::Red);

	}
}