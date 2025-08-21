// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Weapons/LeviathanAxe.h"

#include "RagnarokEngine/Systems/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokContent/Characters/Kratos/Weapons/DataAssets/ItemPrimaryAssetKratosWeapon.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "GameFramework/ProjectileMovementComponent.h"

ALeviathanAxe::ALeviathanAxe()
{
	WeaponRotTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("WeaponRotTimeline"));

	WeaponRotTimelineTick.BindUFunction(this, FName("OnWeaponRotTimelineTick"));
	WeaponRotTimelineEnd.BindUFunction(this, FName("OnWeaponRotTimelineEnd"));

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
	SetActorRotation(NewRotation);
}

void ALeviathanAxe::OnWeaponRotTimelineEnd()
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

void ALeviathanAxe::ThrowWeapon(FRotator CameraRotator, FVector ThrowDirection, FVector CameraLocation, float ThrowSpeed)
{
	//FRotator CalcCameraRotator = FRotator(CameraRotator.Pitch, CameraRotator.Yaw, CameraRotator.Roll + AxeSpinAxisOffset);
	//SnapAxeLocationAndRotation(CalcCameraRotator, ThrowDirection, CameraLocation);

	//ProjectileMovementComponent->SetVelocityInLocalSpace(ThrowDirection * ThrowSpeed);
	//ProjectileMovementComponent->Activate();

	RotateAxe();
}

void ALeviathanAxe::SnapAxeLocationAndRotation(FRotator SnapRotation, FVector SnapDirection, FVector CameraLocation)
{
	SnapDirection.GetSafeNormal();
	FVector SnapLocation = (SnapDirection * ThrowDistance) + CameraLocation;
	SnapLocation -= PivotPointComponent->GetRelativeLocation();
	SetActorLocationAndRotation(SnapLocation, SnapRotation);
}

void ALeviathanAxe::RotateAxe()
{
	if (nullptr != WeaponRotTimelineComponent)
	{
		WeaponRotTimelineComponent->SetPlayRate(WeaponSpinRate);
		WeaponRotTimelineComponent->PlayFromStart();
	}
}
