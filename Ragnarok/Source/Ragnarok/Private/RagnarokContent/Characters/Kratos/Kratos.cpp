// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosControlComponent.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosUIComponent.h"
#include "RagnarokContent/Characters/Kratos/DataAssets/CharacterPrimaryAssetKratos.h"
#include "RagnarokContent/Characters/Kratos/DataAssets/InitDataAssetKratos.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"
#include "RagnarokContent/Characters/Kratos/Animation/KratosLinkedAnimLayer.h"


#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/InputSystem/DataAssets/InputConfigDataAsset.h"
#include "RagnarokEngine/Systems/AbilitySystem/DataAssets/StartUpDataAsset.h"
#include "RagnarokEngine/Systems/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokEngine/Systems/InputSystem/RagnarokEnhancedInputComponent.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAttributeSet.h"

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
	KratosControlComponent = CreateDefaultSubobject<UKratosControlComponent>(TEXT("KratosControl"));
}

void AKratos::BeginPlay()
{
	Super::BeginPlay();

	if (true == bShowDebugMsg) Debug::Print(TEXT("Start Kratos Beginplay method"));

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
	URagnarokEnhancedInputComponent* RagnarokEnhancedInputComponent = 
		KratosControlComponent->SetupPlayerInputComponent(PlayerInputComponent);

	RagnarokEnhancedInputComponent->BindAbilityInputAction(
		InputConfigDA,
		this,
		&ThisClass::InputAbilityPressed,
		&ThisClass::InputAbilityReleased);
}

UCombatComponent* AKratos::GetCombatComponent() const
{
	return KratosCombatComponent;
}

URagnarokUIComponent* AKratos::GetUIComponent() const
{
	return KratosUIComponent;
}

void AKratos::UnEquipWeapon(AKratosWeapon* KratosWeapon)
{
	ULocalPlayer* LocalPlayer = KratosController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	InputSubsystem->RemoveMappingContext(KratosWeapon->WeaponData.InputMappingContext);
	KratosWeapon->UnUequipWeapon(this, UnequipWeaponSocket);
	GetAbilitySystem()->RemoveWeaponAbilities(KratosWeapon->GetGrantedAbilitySpecHandleArray());
	KratosCombatComponent->CurrentEquippedWeaponTag = FGameplayTag::EmptyTag;
	KratosUIComponent->OnEquippedWeaponChanged.Broadcast(nullptr);
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

		
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->RotationRate = InitDA->CharacterMovementRotationRate;
		GetCharacterMovement()->MaxWalkSpeed = InitDA->MaxWalkSpeed;
		KratosControlComponent->RunSpeed = InitDA->MaxRunSpeed;
		KratosControlComponent->WalkSpeed = InitDA->MaxWalkSpeed;
		
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

void AKratos::InputAbilityPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->OnAbilityInputPressed(InputTag);
}

void AKratos::InputAbilityReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->OnAbilityInputReleased(InputTag);
}

bool AKratos::IsRunning() const
{
	return KratosControlComponent->bRunning;
}

FVector2D AKratos::GetMovementInputVector() const
{
	return KratosControlComponent->MovementInputVector;
}

AKratosController* AKratos::GetKratosController()
{
	if (false == KratosController.IsValid())
	{
		KratosController = Cast<AKratosController>(GetController());
	}

	return KratosController.Get();
}
