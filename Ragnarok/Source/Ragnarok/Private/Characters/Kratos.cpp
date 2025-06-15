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
#include "GAS/RagnarokAbilitySystemComponent.h"
#include "GAS/AttributeSets/RagnarokAttributeSet.h"

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

	//LoadKratosPrimaryDataAsset();
	URagnarokAssetManager::Get().LoadPrimaryAssetData(EPrimaryAssetType::EPT_Chracter_Kratos, [this](UObject* LoadedPDA)
		{
			LoadKratosPrimaryDataAsset(LoadedPDA);
		});

	//UCharacterPrimaryAssetKratos* ChracterPDA = Cast<UCharacterPrimaryAssetKratos>(URagnarokAssetManager::Get().GetPDA(EPrimaryAssetType::EPT_Chracter_Kratos));

	//if (nullptr != CharacterPDA)
	//{
	//	if (USkeletalMesh* KratosMesh = CharacterPDA->KratosSkeletalMesh.Get())
	//	{
	//		GetMesh()->SetSkeletalMesh(KratosMesh);
	//	}
	//}
	//else
	//{
	//	Debug::Print(TEXT("CharacterPDA assets is nullptr!!"), FColor::Red);
	//}

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

void AKratos::LoadKratosPrimaryDataAsset(UObject* PDAAssetObject)
{
	CharacterPDA = Cast<UCharacterPrimaryAssetKratos>(PDAAssetObject);

	if (nullptr != CharacterPDA)
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(CharacterPDA->KratosSkeletalMesh.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AKratos::AsyncLoadCharacterKratos));
	}
	else
	{
		Debug::Print(TEXT("UItemPrimaryAssetKratosWeapon primary data assets is nullptr!!"), FColor::Red);
	}
}

void AKratos::AsyncLoadCharacterKratos()
{
	if (USkeletalMesh* KratosMesh = CharacterPDA->KratosSkeletalMesh.Get())
	{
		GetMesh()->SetSkeletalMesh(KratosMesh);
	}
}

//void AKratos::LoadKratosPrimaryDataAsset()
//{
//	FString PrimaryAssetType = URagnarokAssetManager::Get().GetPrimaryAssetType(EPrimaryAssetType::EPT_Chracter_Kratos)->ToString();
//	FString PrimaryAssetName = URagnarokAssetManager::Get().GetPrimaryAssetName(EPrimaryAssetType::EPT_Chracter_Kratos)->ToString();
//
//	CharacterPDAId = FPrimaryAssetId(
//		*PrimaryAssetType,
//		*PrimaryAssetName);
//
//	URagnarokAssetManager::Get().LoadPrimaryAsset
//	(
//		CharacterPDAId,
//		{},
//		FStreamableDelegate::CreateUObject(this, &AKratos::AsyncLoadCharacterKratos)
//	);
//}

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

//void AKratos::AsyncLoadCharacterKratos()
//{
//	UObject* AssetObject = UAssetManager::Get().GetPrimaryAssetObject(CharacterPDAId);
//	CharacterPDA = Cast<UCharacterPrimaryAssetKratos>(AssetObject);
//
//	if (nullptr != CharacterPDA && true == CharacterPDA->KratosSkeletalMesh.IsValid())
//	{
//		GetMesh()->SetSkeletalMesh(CharacterPDA->KratosSkeletalMesh.Get());
//	}
//	else if (nullptr != CharacterPDA)
//	{
//		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
//		StreamableManager.RequestAsyncLoad
//		(
//			CharacterPDA->KratosSkeletalMesh.ToSoftObjectPath(),
//			FStreamableDelegate::CreateLambda([this]()
//				{
//					if (USkeletalMesh* Mesh = CharacterPDA->KratosSkeletalMesh.Get())
//					{
//						GetMesh()->SetSkeletalMesh(Mesh);
//					}
//				}));
//	}
//	else
//	{
//		Debug::Print(TEXT("Kratos config primary data assets is nullptr!!"), FColor::Red);
//
//	}
//}