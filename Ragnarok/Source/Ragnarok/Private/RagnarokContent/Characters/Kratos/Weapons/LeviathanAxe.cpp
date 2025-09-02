// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Weapons/LeviathanAxe.h"

#include "RagnarokEngine/Systems/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokContent/Characters/Kratos/Weapons/DataAssets/ItemPrimaryAssetKratosWeapon.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ALeviathanAxe::ALeviathanAxe()
{
	PrimaryActorTick.bCanEverTick = true;

	PivotPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PivotPoint"));
	PivotPointComponent->SetupAttachment(GetRootComponent());

	LodgePointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LodgePoint"));
	LodgePointComponent->SetupAttachment(PivotPointComponent);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(LodgePointComponent);

	WeaponRotTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("WeaponRotTimeline"));
	WeaponRotTimelineTick.BindUFunction(this, FName("OnWeaponRotTimelineTick"));
	WeaponRotTimelineEnd.BindUFunction(this, FName("OnWeaponRotTimelineEnd"));

	WeaponThrowTraceTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("WeaponThrowTraceTimeline"));
	WeaponThrowTraceTimelineTick.BindUFunction(this, FName("OnWeaponThrowTraceTimelineTick"));
	WeaponThrowTraceTimelineEnd.BindUFunction(this, FName("OnWeaponThrowTraceTimelineEnd"));

	ThrowNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ThrowParticle"));
	ThrowNiagaraComponent->SetupAttachment(WeaponMesh);
}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();

	FOnPrimaryAssetLoadedDelegate PrimaryAssetDataDelegate;

	PrimaryAssetDataDelegate.BindUObject(this, &ALeviathanAxe::LoadWeaponPrimaryDataAsset);

	URagnarokAssetManager::Get().LoadPrimaryAssetData(
		EPrimaryAssetType::EPT_Item_LeviathanAxe,
		PrimaryAssetDataDelegate);

	if (nullptr != WeaponRotationCurve)
	{
		WeaponRotTimelineComponent->AddInterpFloat(WeaponRotationCurve, WeaponRotTimelineTick);
		WeaponRotTimelineComponent->SetTimelineFinishedFunc(WeaponRotTimelineEnd);
		WeaponRotTimelineComponent->SetPlayRate(WeaponSpinRate);
	}

	if (nullptr != WeaponThrowTraceCurve)
	{
		WeaponThrowTraceTimelineComponent->AddInterpFloat(WeaponThrowTraceCurve, WeaponThrowTraceTimelineTick);
		WeaponThrowTraceTimelineComponent->SetTimelineFinishedFunc(WeaponThrowTraceTimelineEnd);
	}

	CurWeaponState = ERagnarokWeaponState::ERWS_Unequipped;
}

void ALeviathanAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALeviathanAxe::InitWeapon()
{
	Super::InitWeapon();

	USkeletalMesh* LoadedWeaponMesh = KratosWeaponPDA->WeaponMesh.Get();

	if (nullptr != LoadedWeaponMesh)
	{
		GetWeaponMesh()->SetSkeletalMesh(LoadedWeaponMesh);
	}
	bInitialized = true;
	OnWeaponInitialized.Broadcast();
}

void ALeviathanAxe::OnWeaponRotTimelineTick(float Value)
{
	float RotationValue = Value * -360.0f;
	FRotator NewRotation = FRotator(RotationValue, 0.0f, 0.0f);
	PivotPointComponent->SetRelativeRotation(NewRotation);

	ThrowFlipFlopTime += GetWorld()->GetDeltaSeconds();

	if (ThrowFlipFlopTime >= 0.1f)
	{
		USoundBase* SoundToPlay = ThrowSoundMap[bThrowSoundFlipFlop];
		bThrowSoundFlipFlop = !bThrowSoundFlipFlop;

		if (nullptr != SoundToPlay && nullptr != ThrowSoundAttenuation)
		{
			UGameplayStatics::SpawnSoundAttached(
				SoundToPlay,
				GetWeaponMesh(),
				NAME_None,
				FVector::ZeroVector,
				EAttachLocation::SnapToTarget,
				false,
				1.15f,
				1.0f,
				0.0f,
				ThrowSoundAttenuation
			);
		}

		ThrowFlipFlopTime = 0.0f;
	}
}

void ALeviathanAxe::OnWeaponRotTimelineEnd()
{
}

void ALeviathanAxe::OnWeaponThrowTraceTimelineTick(float Value)
{
	ProjectileMovementComponent->ProjectileGravityScale = Value;
}

void ALeviathanAxe::OnWeaponThrowTraceTimelineEnd()
{
}


void ALeviathanAxe::LoadWeaponDataAsset()
{
	Super::LoadWeaponDataAsset();
}

void ALeviathanAxe::LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject)
{
	Super::LoadWeaponPrimaryDataAsset(PDAAssetObject);

	UItemPrimaryAssetKratosWeapon* LoadedPDA = Cast<UItemPrimaryAssetKratosWeapon>(PDAAssetObject);

	if (nullptr == LoadedPDA)
	{
		Debug::Print(TEXT("Kratos Weapon PDA is nullptr!!"), FColor::Red);
		return;
	}

	KratosWeaponPDA = LoadedPDA;

	if (nullptr == KratosWeaponPDA->WeaponMesh)
	{
		FSoftObjectPath MeshPath = KratosWeaponPDA->WeaponMesh.ToSoftObjectPath();

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			MeshPath,
			FStreamableDelegate::CreateLambda([this]()
				{
					if (USkeletalMesh* LoadedWeaponMesh = KratosWeaponPDA->WeaponMesh.Get())
					{
						GetWeaponMesh()->SetSkeletalMesh(LoadedWeaponMesh);
					}
				})
		);
	}
	else
	{
		if (USkeletalMesh* LoadedWeaponMesh = KratosWeaponPDA->WeaponMesh.Get())
		{
			GetWeaponMesh()->SetSkeletalMesh(LoadedWeaponMesh);
		}
	}
}

void ALeviathanAxe::ThrowWeapon(FRotator CameraRotation, FVector CameraLocation, FVector CameraForwardVector)
{
	CurWeaponState = ERagnarokWeaponState::ERWS_Throw;

	FRotator CalcCameraRotator = FRotator(CameraRotation.Pitch, CameraRotation.Yaw, CameraRotation.Roll + AxeSpinAxisOffset);
	SnapAxeLocationAndRotation(CalcCameraRotator, CameraLocation, CameraForwardVector);

	ProjectileMovementComponent->Velocity = CameraForwardVector * ThrowSpeed;
	ProjectileMovementComponent->Activate();
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	if (nullptr != WeaponRotTimelineComponent)
	{
		WeaponRotTimelineComponent->SetPlayRate(WeaponSpinRate);
		WeaponRotTimelineComponent->PlayFromStart();
		WeaponRotTimelineComponent->SetLooping(true);
	}

	if (nullptr != WeaponThrowTraceTimelineComponent)
	{
		WeaponThrowTraceTimelineComponent->PlayFromStart();
	}

	StartWeaponTrail();
}

void ALeviathanAxe::RecallWeapon()
{
	WeaponThrowTraceTimelineComponent->Stop();
}

void ALeviathanAxe::SnapAxeLocationAndRotation(FRotator StartRotation, FVector CameraLocation, FVector CameraForwardVector)
{
	FVector StartLocation = (CameraForwardVector * 250.0f) + CameraLocation;
	StartLocation -= PivotPointComponent->GetRelativeLocation();
	SetActorLocationAndRotation(StartLocation, StartRotation);
}

void ALeviathanAxe::RotateAxe()
{
	if (nullptr != WeaponRotTimelineComponent)
	{
		WeaponRotTimelineComponent->SetPlayRate(WeaponSpinRate);
		WeaponRotTimelineComponent->PlayFromStart();
	}
}

void ALeviathanAxe::StartWeaponTrail()
{
	if (nullptr != ThrowNiagaraComponent)
	{
		ThrowNiagaraComponent->Activate();
	}
}
