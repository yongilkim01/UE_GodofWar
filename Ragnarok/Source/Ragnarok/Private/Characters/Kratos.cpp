// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Kratos.h"
#include "Tools/RagnarokDebugHelper.h"

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Data/Kratos/CharacterPrimaryAssetKratos.h"
#include "Data/Kratos/InitDataAssetKratos.h"
#include "Manager/Global/RagnarokAssetManager.h"
#include "Types/RagnarokTypes.h"

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

	LoadKratosDataAsset();
	LoadKratosPrimaryDataAsset();
}

void AKratos::LoadKratosDataAsset()
{
	if (nullptr != InitDA)
	{
		GetCapsuleComponent()->InitCapsuleSize(InitDA->CapsuleRadius, InitDA->CapsuleHalfHeight);

		SpringArmComponent->TargetArmLength = InitDA->TargetArmLength;
		SpringArmComponent->SocketOffset = InitDA->SocketOffset;

		GetCharacterMovement()->RotationRate = InitDA->CharacterMovementRotationRate;
		GetCharacterMovement()->MaxWalkSpeed = InitDA->MaxWalkSpeed;

		GetMesh()->SetRelativeLocation(InitDA->SkeletalMeshOffset);
		GetMesh()->SetRelativeRotation(InitDA->SkeletalMeshRotator);
	}
	else
	{
		Debug::Print(TEXT("Init data assets is nullptr!!"), FColor::Red);
	}
}

void AKratos::LoadKratosPrimaryDataAsset()
{
	FString PrimaryAssetType = URagnarokAssetManager::Get().GetPrimaryAssetType(EPrimaryAssetType::EPT_CHARACTER_KRATOS)->ToString();
	FString PrimaryAssetName = URagnarokAssetManager::Get().GetPrimaryAssetName(EPrimaryAssetType::EPT_CHARACTER_KRATOS)->ToString();

	CharacterPDAId = FPrimaryAssetId(
		*PrimaryAssetType,
		*PrimaryAssetName);

	URagnarokAssetManager::Get().LoadPrimaryAsset
	(
		CharacterPDAId,
		{},
		FStreamableDelegate::CreateUObject(this, &AKratos::AsyncLoadCharacterKratos)
	);
}

void AKratos::AsyncLoadCharacterKratos()
{
	UObject* AssetObject = UAssetManager::Get().GetPrimaryAssetObject(CharacterPDAId);
	CharacterPDA = Cast<UCharacterPrimaryAssetKratos>(AssetObject);

	if (nullptr != CharacterPDA && true == CharacterPDA->KratosSkeletalMesh.IsValid())
	{
		GetMesh()->SetSkeletalMesh(CharacterPDA->KratosSkeletalMesh.Get());
	}
	else if (nullptr != CharacterPDA)
	{
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad
		(
			CharacterPDA->KratosSkeletalMesh.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([this]()
				{
					if (USkeletalMesh* Mesh = CharacterPDA->KratosSkeletalMesh.Get())
					{
						GetMesh()->SetSkeletalMesh(Mesh);
					}
				}));
	}
	else
	{
		Debug::Print(TEXT("Kratos config primary data assets is nullptr!!"), FColor::Red);

	}
}