// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/KratosWeapon.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosCombatComponent.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosControlComponent.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosSpringArmComponent.h"
#include "RagnarokContent/Characters/Kratos/Components/KratosUIComponent.h"
#include "RagnarokContent/Characters/Kratos/DataAssets/CharacterPrimaryAssetKratos.h"
#include "RagnarokContent/Characters/Kratos/DataAssets/InitDataAssetKratos.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"
#include "RagnarokContent/Characters/Kratos/Animation/KratosLinkedAnimLayer.h"

#include "RagnarokEngine/Camera/RagnarokSpringArmComponent.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Input/DataAssets/InputConfigDataAsset.h"
#include "RagnarokContent/Abilities/DataAssets/StartUpDataAsset.h"
#include "RagnarokEngine/Asset/RagnarokAssetManager.h"
#include "RagnarokEngine/Input/RagnarokEnhancedInputComponent.h"
#include "RagnarokEngine/Core/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Core/GameplayAbilities/RagnarokAttributeSet.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"



AKratos::AKratos()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<UKratosSpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	CameraZoomTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraZoomTimeline"));

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

	if (nullptr != CameraZoomCurve && nullptr != CameraZoomTimelineComponent)
	{
		FOnTimelineFloat TimelineDelegateFloat;
		TimelineDelegateFloat.BindUFunction(this, FName("OnCameraZoomTimelineTick"));
		CameraZoomTimelineComponent->AddInterpFloat(CameraZoomCurve, TimelineDelegateFloat);

		FOnTimelineEvent TimelineDelegateEvent;
		TimelineDelegateEvent.BindUFunction(this, FName("OnCameraZoomTimelineEnd"));
		CameraZoomTimelineComponent->SetTimelineFinishedFunc(TimelineDelegateEvent);

		CameraZoomTimelineComponent->SetPlayRate(TimelineComponentPlayRate);
	}
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

void AKratos::EquipWeaponToAttackSocekt(AKratosWeapon* KratosWeapon)
{
	USkeletalMeshComponent* ParentMesh = nullptr;
	ParentMesh = FindComponentByClass<USkeletalMeshComponent>();

	if (nullptr != ParentMesh)
	{
		FAttachmentTransformRules AttachRules(
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			true
		);

		KratosWeapon->AttachToComponent(
			ParentMesh,
			AttachRules,
			EquipWeaponAttackSocket
		);
	}
}

void AKratos::UnEquipWeaponToAttackSocekt(AKratosWeapon* KratosWeapon)
{
	USkeletalMeshComponent* ParentMesh = nullptr;
	ParentMesh = FindComponentByClass<USkeletalMeshComponent>();

	if (nullptr != ParentMesh)
	{
		FAttachmentTransformRules AttachRules(
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			true
		);

		KratosWeapon->AttachToComponent(
			ParentMesh,
			AttachRules,
			EquipWeaponSocket
		);
	}
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
		SpringArmComponent->bInheritPitch = true;
		SpringArmComponent->bInheritYaw = true;
		SpringArmComponent->bInheritRoll = false;
		SpringArmComponent->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

		MainCameraComponent->bUsePawnControlRotation = false;

		GetKratosController()->PlayerCameraManager->ViewPitchMax = 60.0f;
		GetKratosController()->PlayerCameraManager->ViewPitchMin = -50.0f;

		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->RotationRate = InitDA->CharacterMovementRotationRate;
		//GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->MaxWalkSpeed = InitDA->MaxWalkSpeed;

		GetMesh()->SetRelativeLocation(InitDA->SkeletalMeshOffset);
		GetMesh()->SetRelativeRotation(InitDA->SkeletalMeshRotator);
	
		IdleSpringArmLength = InitDA->TargetArmLength;
		CameraZoomSpringArmLength = InitDA->ZoomTargetArmLength;

		IdleMaxWalkSpeed = InitDA->MaxWalkSpeed;
		CameraZoomMaxWalkSpeed = InitDA->MaxZoomWalkSpeed;;

		KratosControlComponent->RunSpeed = InitDA->MaxRunSpeed;
		KratosControlComponent->WalkSpeed = InitDA->MaxWalkSpeed;

		EquipWeaponAttackSocket = FName("RightWeaponAttackSocket");
		EquipWeaponSocket = FName("RightWeaponSocket");
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

bool AKratos::IsAiming() const
{
	return GetAbilitySystemComponent()->HasMatchingGameplayTag(KratosGameplayTags::Kratos_Status_Aiming);
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

ARagnarokWeapon* AKratos::GetKratosWeapon()
{
	return KratosCombatComponent->GetCurrentEquippedWeapon();
}

bool AKratos::GetAimingTargetLocation(FVector InWeaponLocation, FVector& OutTargetLocation, float MaxRange)
{
	FVector CameraForward = MainCameraComponent->GetForwardVector();

	FHitResult HitResult;
	FVector StartLocation = InWeaponLocation;
	FVector TargetLocation = StartLocation + (CameraForward * MaxRange);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		TargetLocation,
		ECC_Visibility,
		QueryParams
	);

	if (true == bHit)
	{
		OutTargetLocation = HitResult.Location;
	}
	else
	{
		OutTargetLocation = TargetLocation;
	}

	return bHit;
}

void AKratos::ThrowWeapon(AKratosWeapon* KratosWeapon)
{
	FVector CameraForwardVector = MainCameraComponent->GetForwardVector();
	FVector CameraLocation = MainCameraComponent->GetComponentLocation();
	FRotator CameraRotation = MainCameraComponent->GetComponentRotation();

	KratosCombatComponent->ToggleWeaponCollision(true);

	KratosWeapon->ThrowWeapon(CameraRotation, CameraLocation, CameraForwardVector);

}

void AKratos::ZoomInCamera()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->MaxWalkSpeed = CameraZoomMaxWalkSpeed;

	if (nullptr != CameraZoomTimelineComponent)
	{
		CameraZoomTimelineComponent->Play();
	}
}

void AKratos::ZoomOutCamera()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = IdleMaxWalkSpeed;

	if (nullptr != CameraZoomTimelineComponent)
	{
		CameraZoomTimelineComponent->Reverse();
	}
}

void AKratos::ShakeCamera()
{
	if (false == CameraZoomTimelineComponent->IsPlaying())
	{
		GetKratosController()->PlayerCameraManager->StartCameraShake(ShakeCameraClass, 1.0f);
	}
}


void AKratos::OnCameraZoomTimelineTick(float Value)
{
	if (nullptr != SpringArmComponent)
	{
		float CalcLength = FMath::Lerp(IdleSpringArmLength, CameraZoomSpringArmLength, Value);
		SpringArmComponent->TargetArmLength = CalcLength;
	}
}

void AKratos::OnCameraZoomTimelineEnd()
{
}
