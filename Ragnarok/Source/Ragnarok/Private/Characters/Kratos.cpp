// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Kratos.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Core/Types/RagnarokTypes.h"
#include "Core/Tags/RagnarokGameplayTags.h"
#include "Core/Tools/RagnarokDebugHelper.h"
#include "Data/Kratos/CharacterPrimaryAssetKratos.h"
#include "Data/Kratos/InitDataAssetKratos.h"
#include "Data/Input/InputConfigDataAsset.h"
#include "Manager/Global/RagnarokAssetManager.h"
#include "Components/Input/RagnarokEnhancedInputComponent.h"

AKratos::AKratos()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
}

void AKratos::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("Start Kratos Beginplay method"));

	LoadKratosDataAsset();
	LoadKratosPrimaryDataAsset();
}

void AKratos::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDA, TEXT("Check input config data asset"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(InputSubsystem);

	InputSubsystem->AddMappingContext(InputConfigDA->InputMappingContext, 0);

	URagnarokEnhancedInputComponent* RagnarokInputComponent
		= CastChecked<URagnarokEnhancedInputComponent>(PlayerInputComponent);

	RagnarokInputComponent->BindNativeInputAction(
		InputConfigDA,
		RagnarokGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::InputMove
	);
}

void AKratos::LoadKratosDataAsset()
{
	if (nullptr != InitDA)
	{
		GetCapsuleComponent()->InitCapsuleSize(InitDA->CapsuleRadius, InitDA->CapsuleHalfHeight);

		SpringArmComponent->TargetArmLength = InitDA->TargetArmLength;
		SpringArmComponent->SocketOffset = InitDA->SocketOffset;	
		SpringArmComponent->bUsePawnControlRotation = true;

		MainCameraComponent->bUsePawnControlRotation = false;

		GetCharacterMovement()->bOrientRotationToMovement = true;
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

void AKratos::InputMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	if (0.0f != MovementVector.Y)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}


	if (0.0f != MovementVector.X)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
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