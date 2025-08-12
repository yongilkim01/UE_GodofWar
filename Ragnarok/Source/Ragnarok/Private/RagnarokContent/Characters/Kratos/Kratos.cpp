// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Kratos.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/InputSystem/DataAssets/InputConfigDataAsset.h"
#include "RagnarokEngine/Systems/AbilitySystem/DataAssets/StartUpDataAsset.h"
#include "RagnarokEngine/Systems/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokEngine/Systems/InputSystem/RagnarokEnhancedInputComponent.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAttributeSet.h"

#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosUIComponent.h"
#include "RagnarokContent/Characters/Kratos/DataAssets/CharacterPrimaryAssetKratos.h"
#include "RagnarokContent/Characters/Kratos/DataAssets/InitDataAssetKratos.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

AKratos::AKratos()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	KratosCombatComponent = CreateDefaultSubobject<UKratosCombatComponent>(TEXT("KratosCombat"));
	KratosUIComponent = CreateDefaultSubobject<UKratosUIComponent>(TEXT("KratosUI"));
}

void AKratos::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print(TEXT("Start Kratos Beginplay method"));

	FOnPrimaryAssetLoadedDelegate PrimaryAssetDataDelegate;

	PrimaryAssetDataDelegate.BindUObject(this, &AKratos::InitPrimaryData);

	URagnarokAssetManager::Get().LoadPrimaryAssetData(
		EPrimaryAssetType::EPT_Chracter_Kratos,
		PrimaryAssetDataDelegate);

	LoadKratosDataAsset();

	FixedCameraWorldZLocation = MainCameraComponent->GetComponentLocation().Z;
}

void AKratos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	check(RagnarokInputComponent);

	RagnarokInputComponent->BindNativeInputAction(
		InputConfigDA,
		RagnarokGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::InputMove
	);

	RagnarokInputComponent->BindNativeInputAction(
		InputConfigDA,
		RagnarokGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::InputLook
	);

	RagnarokInputComponent->BindNativeInputAction(
		InputConfigDA,
		KratosGameplayTags::InputTag_Run,
		ETriggerEvent::Started,
		this,
		&ThisClass::InputStartRun
	);

	RagnarokInputComponent->BindNativeInputAction(
		InputConfigDA,
		KratosGameplayTags::InputTag_Run,
		ETriggerEvent::Completed,
		this,
		&ThisClass::InputEndRun
	);

	RagnarokInputComponent->BindAbilityInputAction(
		InputConfigDA,
		this,
		&ThisClass::InputAbilityPressed,
		&ThisClass::InputAbilityReleased);
}

void AKratos::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (nullptr != AbilitySystemComponent && nullptr != AttributeSet)
	{
		const FString DebugInfoStr = 
			FString::Printf(TEXT("GAS Owner Actor : %s, AvataActor : %s"), 
			*AbilitySystemComponent->GetOwnerActor()->GetActorNameOrLabel(),
			*AbilitySystemComponent->GetAvatarActor()->GetActorNameOrLabel());

		Debug::Print(TEXT("Ability system component valid ") + DebugInfoStr, FColor::Green);
		Debug::Print(TEXT("AttributeSet valid ") + DebugInfoStr, FColor::Green);		
	}
}

UCombatComponent* AKratos::GetCombatComponent() const
{
	return KratosCombatComponent;
}

URagnarokUIComponent* AKratos::GetUIComponent() const
{
	return KratosUIComponent;
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

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->RotationRate = InitDA->CharacterMovementRotationRate;
		GetCharacterMovement()->MaxWalkSpeed = InitDA->MaxWalkSpeed;
		RunSpeed = InitDA->MaxRunSpeed;
		WalkSpeed = InitDA->MaxWalkSpeed;
		
		// 루트모션 Z축 이동을 위한 설정
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
		//GetCharacterMovement()->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
		
		GetMesh()->SetRelativeLocation(InitDA->SkeletalMeshOffset);
		GetMesh()->SetRelativeRotation(InitDA->SkeletalMeshRotator);
	}
	else
	{
		Debug::Print(TEXT("Init data assets is nullptr!!"), FColor::Red);
	}
}

void AKratos::InitPrimaryData(UObject* PDAObject)
{
	UCharacterPrimaryAssetKratos* LoadedPDA = Cast<UCharacterPrimaryAssetKratos>(PDAObject);

	if (nullptr == LoadedPDA)
	{
		Debug::Print(TEXT("Character PDA is nullptr!!"), FColor::Red);
		return;
	}

	if (nullptr == LoadedPDA->KratosSkeletalMesh)
	{
		FSoftObjectPath MeshPath = LoadedPDA->KratosSkeletalMesh.ToSoftObjectPath();

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			MeshPath,
			FStreamableDelegate::CreateLambda([this, LoadedPDA]()
				{
					if (USkeletalMesh* KratosMesh = LoadedPDA->KratosSkeletalMesh.Get())
					{
						GetMesh()->SetSkeletalMesh(KratosMesh);

						if (false == StartUpData.IsNull())
						{
							UStartUpDataAsset* LoadedData = StartUpData.LoadSynchronous();

							if (nullptr != LoadedData)
							{
								LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
							}
						}
					}
				})
		);
	}
	else
	{
		if (USkeletalMesh* KratosMesh = LoadedPDA->KratosSkeletalMesh.Get())
		{
			GetMesh()->SetSkeletalMesh(KratosMesh);

			if (false == StartUpData.IsNull())
			{
				UStartUpDataAsset* LoadedData = StartUpData.LoadSynchronous();

				if (nullptr != LoadedData)
				{
					LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
				}
			}
		}
	}
}

void AKratos::InputMove(const FInputActionValue& InputActionValue)
{
	CachedMovementInputVector = InputActionValue.Get<FVector2D>();

	if (true == bRolling || true == bAttacking) return;

	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	if (0.0f != MovementVector.Y)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		if (MovementVector.Y > 0.0f)
		{
			FRotator CurrentRotation = GetActorRotation();
			FRotator TargetRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

			float RotationInterpSpeed = 15.0f;
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationInterpSpeed);
			SetActorRotation(TargetRotation);
		}
	}


	if (0.0f != MovementVector.X)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AKratos::InputLook(const FInputActionValue& InputActionValue)
{
	if (true == bRolling || true == bAttacking) return;

	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if(0.0f != LookAxisVector.X)
	{
		AddControllerYawInput(LookAxisVector.X);
	}
	if (0.0f != LookAxisVector.Y)
	{
		AddControllerPitchInput(-LookAxisVector.Y);
	}

}

void AKratos::InputStartRun(const FInputActionValue& InputActionValue)
{
	bRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AKratos::InputEndRun(const FInputActionValue& InputActionValue)
{
	bRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AKratos::InputAbilityPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->OnAbilityInputPressed(InputTag);
}

void AKratos::InputAbilityReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->OnAbilityInputReleased(InputTag);
}
