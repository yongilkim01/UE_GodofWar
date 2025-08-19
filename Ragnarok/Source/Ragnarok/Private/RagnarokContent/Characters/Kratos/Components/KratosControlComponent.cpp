// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Components/KratosControlComponent.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/InputSystem/DataAssets/InputConfigDataAsset.h"
#include "RagnarokEngine/Systems/InputSystem/RagnarokEnhancedInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"


UKratosControlComponent::UKratosControlComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UKratosControlComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UKratosControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalcCharacterRotation(DeltaTime);

}

void UKratosControlComponent::InitControlComponent()
{
	Kratos = Cast<AKratos>(GetOwner());

	if (nullptr != Kratos)
	{
		KratosController = Cast<AKratosController>(Kratos->GetController());

		if (nullptr == KratosController)
		{
			Debug::Print(TEXT("UKratosControlComponent::KratosController is nullptr"), FColor::Red);
		}
	}
	else
	{
		Debug::Print(TEXT("UKratosControlComponent::Kratos is nullptr!"), FColor::Red);
	}
}

URagnarokEnhancedInputComponent* UKratosControlComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDA, TEXT("Check input config data asset"));

	InitControlComponent();

	ULocalPlayer* LocalPlayer = Kratos->GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(InputSubsystem);

	InputSubsystem->AddMappingContext(InputConfigDA->InputMappingContext, 0);

	URagnarokEnhancedInputComponent* RagnarokEnhancedInputComponent
		= CastChecked<URagnarokEnhancedInputComponent>(PlayerInputComponent);

	check(RagnarokEnhancedInputComponent);

	RagnarokEnhancedInputComponent->BindNativeInputAction(
		InputConfigDA,
		RagnarokGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::InputMovePressed
	);

	RagnarokEnhancedInputComponent->BindNativeInputAction(
		InputConfigDA,
		RagnarokGameplayTags::InputTag_Move,
		ETriggerEvent::Completed,
		this,
		&ThisClass::InputMoveReleased
	);

	RagnarokEnhancedInputComponent->BindNativeInputAction(
		InputConfigDA,
		RagnarokGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::InputLook
	);

	RagnarokEnhancedInputComponent->BindNativeInputAction(
		InputConfigDA,
		KratosGameplayTags::InputTag_Run,
		ETriggerEvent::Started,
		this,
		&ThisClass::InputRun
	);

	return RagnarokEnhancedInputComponent;
}

void UKratosControlComponent::CalcCharacterRotation(float DeltaTime)
{
	if (false == IsMoving())
	{
		return;
	}

	FRotator TargetRotation = FRotator::ZeroRotator;
	FRotator KratosRotation = Kratos->GetActorRotation();

	if (true == bUseControllerRotation)
	{
		TargetRotation = FRotator(0.0f, KratosController->GetControlRotation().Yaw, 0.0f);
	}
	else
	{
		TargetRotation = FRotator(0.0f, KratosRotation.Yaw, 0.0f);
	}

	FRotator CalcRotation = FMath::RInterpTo(KratosRotation, TargetRotation, DeltaTime, InterpolateSpeed);
	Kratos->SetActorRotation(CalcRotation, ETeleportType::TeleportPhysics);

}

bool UKratosControlComponent::IsMoving() const
{
	if (nullptr == Kratos)
	{
		return false;
	}

	return Kratos->GetVelocity().Size() > MovementThreshold;
}

FRotator UKratosControlComponent::GetTargetRotation() const
{
	return FRotator();
}

void UKratosControlComponent::InputMovePressed(const FInputActionValue& InputActionValue)
{
	MovementInputVector = InputActionValue.Get<FVector2D>();

	if (true == Kratos->bRolling || true == Kratos->bAttacking) return;

	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, KratosController->GetControlRotation().Yaw, 0.0f);

	if (0.0f != MovementVector.Y)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		Kratos->AddMovementInput(ForwardDirection, MovementVector.Y);

		if (MovementVector.Y > 0.0f)
		{
			FRotator CurrentRotation = Kratos->GetActorRotation();
			FRotator TargetRotation = FRotator(0.0f, KratosController->GetControlRotation().Yaw, 0.0f);

			float RotationInterpSpeed = 15.0f;
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationInterpSpeed);
			Kratos->SetActorRotation(TargetRotation);
		}
	}


	if (0.0f != MovementVector.X)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		Kratos->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UKratosControlComponent::InputMoveReleased(const FInputActionValue& InputActionValue)
{
	bRunning = false;
	Kratos->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void UKratosControlComponent::InputLook(const FInputActionValue& InputActionValue)
{
	if (true == Kratos->bRolling || true == Kratos->bAttacking) return;

	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (0.0f != LookAxisVector.X)
	{
		Kratos->AddControllerYawInput(LookAxisVector.X);
	}
	if (0.0f != LookAxisVector.Y)
	{
		Kratos->AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void UKratosControlComponent::InputRun(const FInputActionValue& InputActionValue)
{
	bRunning = true;
	Kratos->GetCharacterMovement()->MaxWalkSpeed = bRunning ? RunSpeed : WalkSpeed;
}
