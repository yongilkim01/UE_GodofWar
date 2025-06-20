// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Kratos.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokContent/Characters/Kratos/DataAssets/CharacterPrimaryAssetKratos.h"
#include "RagnarokContent/Characters/Kratos/DataAssets/InitDataAssetKratos.h"
#include "RagnarokEngine/DataAssets/InputConfigDataAsset.h"
#include "RagnarokEngine/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokEngine/InputSystem/RagnarokEnhancedInputComponent.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokAttributeSet.h"
#include "RagnarokEngine/DataAssets/StartUpData/StartUpDataAsset.h"

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

	FOnPrimaryAssetLoadedDelegate PrimaryAssetDataDelegate;

	PrimaryAssetDataDelegate.BindUObject(this, &AKratos::InitPrimaryData);

	URagnarokAssetManager::Get().LoadPrimaryAssetData(
		EPrimaryAssetType::EPT_Chracter_Kratos,
		PrimaryAssetDataDelegate);

	LoadKratosDataAsset();
}

void AKratos::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDA, TEXT("Check input config data asset"));

	// 현재 컨트롤러가 APlayerController인지 확인 후, 그에 해당하는 LocalPlayer를 가져옴
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	// EnhancedInput 시스템을 사용하기 위한 서브시스템을 가져옴
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(InputSubsystem);

	// InputConfigDA에 저장된 MappingContext를 InputSubsystem에 등록 (우선순위 0)
	InputSubsystem->AddMappingContext(InputConfigDA->InputMappingContext, 0);

	// 입력을 처리할 컴포넌트를 캐스팅
	URagnarokEnhancedInputComponent* RagnarokInputComponent
		= CastChecked<URagnarokEnhancedInputComponent>(PlayerInputComponent);

	check(RagnarokInputComponent);

	// InputConfigDA에 정의된 "Move" InputTag에 대해, Triggered 시 InputMove 함수를 바인딩
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
}

void AKratos::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (nullptr != AbilitySystemComponent && nullptr != AttributeSet)
	{
		const FString DebugInfoStr = 
			FString::Printf(TEXT("GAS Owner Actor : %s, AvataActor : %s"), 
			*AbilitySystemComponent->GetOwnerActor()->GetActorLabel(),
			*AbilitySystemComponent->GetAvatarActor()->GetActorLabel());

		Debug::Print(TEXT("Ability system component valid ") + DebugInfoStr, FColor::Green);
		Debug::Print(TEXT("AttributeSet valid ") + DebugInfoStr, FColor::Green);		
	}
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

void AKratos::InputLook(const FInputActionValue& InputActionValue)
{
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