// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Weapons/LeviathanAxe.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"

#include "RagnarokEngine/Systems/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokContent/Characters/Kratos/Weapons/DataAssets/ItemPrimaryAssetKratosWeapon.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"


ALeviathanAxe::ALeviathanAxe()
{
	PrimaryActorTick.bCanEverTick = true;

	PivotPointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PivotPoint"));
	PivotPointComponent->SetupAttachment(GetRootComponent());

	LodgePointComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LodgePoint"));
	LodgePointComponent->SetupAttachment(PivotPointComponent);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(LodgePointComponent);

	WeaponRotTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotTimeline"));
	WeaponRotTimelineTick.BindUFunction(this, FName("OnWeaponRotTimelineTick"));
	WeaponRotTimelineEnd.BindUFunction(this, FName("OnWeaponRotTimelineEnd"));

	WeaponThrowTraceTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("ThrowTraceTimeline"));
	WeaponThrowTraceTimelineTick.BindUFunction(this, FName("OnWeaponThrowTraceTimelineTick"));
	WeaponThrowTraceTimelineEnd.BindUFunction(this, FName("OnWeaponThrowTraceTimelineEnd"));

	WeaponWiggleTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Wiggleimeline"));
	WeaponWiggleTimelineTick.BindUFunction(this, FName("OnWeaponWiggleTimelineTick"));
	WeaponWiggleTimelineEnd.BindUFunction(this, FName("OnWeaponWiggleTimelineEnd"));

	WeaponRecallTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("RecallTimeline"));
	WeaponRecallTimelineTick.BindUFunction(this, FName("OnWeaponRecallTimelineTick"));
	WeaponRecallTimelineEnd.BindUFunction(this, FName("OnWeaponRecallTimelineEnd"));


	BladeNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Blade Niagara"));
	BladeNiagaraComponent->SetupAttachment(WeaponMesh);
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

	if (nullptr != WeaponRecallWiggleCurve)
	{
		WeaponWiggleTimelineComponent->AddInterpFloat(WeaponRecallWiggleCurve, WeaponWiggleTimelineTick);
		WeaponWiggleTimelineComponent->SetTimelineFinishedFunc(WeaponWiggleTimelineEnd);
	}

	if (nullptr != RecallSpeedCurve)
	{
		WeaponRecallTimelineComponent->AddInterpFloat(RecallSpeedCurve, WeaponRecallTimelineTick);
		WeaponRecallTimelineComponent->SetTimelineFinishedFunc(WeaponRecallTimelineEnd);
	}

	CurWeaponState = ERagnarokWeaponState::ERWS_Unequipped;

	InitVFX();
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
	//Debug::Print(TEXT("Throw Rot Tick"));

	float RotationValue = Value * -360.0f;
	FRotator NewRotation = FRotator(RotationValue, 0.0f, 0.0f);
	PivotPointComponent->SetRelativeRotation(NewRotation);

	ThrowFlipFlopTime += GetWorld()->GetDeltaSeconds();

	if (ThrowFlipFlopTime >= 0.1f)
	{
		USoundBase* SoundToPlay = ThrowSoundMap[bThrowSoundFlipFlop];
		bThrowSoundFlipFlop = !bThrowSoundFlipFlop;

		if (nullptr != SoundToPlay && nullptr != AttachSoundAttenuation)
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
				AttachSoundAttenuation
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
	Debug::Print(TEXT("Throw Trace Tick Value : "), Value);
	ProjectileMovementComponent->ProjectileGravityScale = Value;
	CheckHitCollision();
}

void ALeviathanAxe::OnWeaponThrowTraceTimelineEnd()
{
	StopWeapon();
}

void ALeviathanAxe::OnWeaponWiggleTimelineTick(float Value)
{
	LodgePointComponent->SetRelativeRotation(FRotator(
		LodgeRotation.Pitch,
		LodgeRotation.Yaw,
		LodgeRotation.Roll +  (Value * 12.0f)
	));
}

void ALeviathanAxe::OnWeaponWiggleTimelineEnd()
{
}

void ALeviathanAxe::OnWeaponRecallTimelineTick(float Value)
{
	float Alpha = WeaponRecallTimelineComponent->GetPlaybackPosition() / WeaponRecallTimelineComponent->GetTimelineLength();

	float SpeedValue = Value;
	float RightVectorValue = RecallRightVectorCurve ? RecallRightVectorCurve->GetFloatValue(Alpha) : 0.0f;
	float Rotation1Value = RecallRotationCurve1 ? RecallRotationCurve1->GetFloatValue(Alpha) : 0.0f;
	float Rotation2Value = RecallRotationCurve2 ? RecallRotationCurve2->GetFloatValue(Alpha) : 0.0f;
	float SoundVolValue = RecallSoundVolCurve ? RecallSoundVolCurve->GetFloatValue(Alpha) : 0.0f;

	FVector CalcVector = (RightVectorValue * (DistanceFromOwner / AxeRightVectorScale)) * OwnerKratos->GetKratosCameraComponent()->GetRightVector();
	CalcVector += OwnerKratos->GetMesh()->GetSocketLocation(TEXT("RightWeaponSocket"));
	RecallTargetLocation = FMath::Lerp(InitLocation, CalcVector, SpeedValue);
	//Debug::Print(TEXT("LerpLocation: ") + RecallTargetLocation.ToString(), FColor::Cyan);
	SetActorLocation(RecallTargetLocation);
}

void ALeviathanAxe::OnWeaponRecallTimelineEnd()
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
	CameraStartRotation = CameraRotation;

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
		WeaponThrowTraceTimelineComponent->SetLooping(true);

	}

	StartWeaponTrail();
}

void ALeviathanAxe::RecallWeapon()
{
	StopWeapon();

	WeaponMesh->SetVisibility(true);

	if (nullptr != RecallNoiseSoundCue)
	{
		RecallAudioComponent = UGameplayStatics::SpawnSoundAttached(
			RecallNoiseSoundCue,
			GetWeaponMesh(),
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::SnapToTargetIncludingScale,
			false,
			0.0f,
			1.0f,
			0.0f,
			AttachSoundAttenuation
		);
	}

	switch (CurWeaponState)
	{
	case ERagnarokWeaponState::ERWS_Throw:
	{
		ZAdjustment = 10.0f;
	}
	break;
	case ERagnarokWeaponState::ERWS_Lodge:
	{
		WiggleLodgedAxe();
	}
	break;
	default:
		break;
	}

	CurWeaponState = ERagnarokWeaponState::ERWS_Recall;
	DistanceFromOwner = GetClampedDistanceFromOwnerCharacter(MaxCalcDistance);
	SetWeaponRecallLocation();
	InitLocation = GetActorLocation();
	InitRotation = GetActorRotation();
	CameraStartRotation = OwnerKratos->GetKratosCameraComponent()->GetComponentRotation();
	LodgePointComponent->SetRelativeRotation(FRotator::ZeroRotator);
	
	if (nullptr != WeaponRecallTimelineComponent)
	{
		float PlayRate = CalcRecallTimelinePlayRate(OptimalDistance, AxeRecallSpeed);
		WeaponRecallTimelineComponent->SetPlayRate(PlayRate);
		WeaponRecallTimelineComponent->PlayFromStart();
	}

	



	//if (nullptr != RecallAudioComponent)
	//{
	//	RecallAudioComponent->FadeOut(0.4f, 0.0f);
	//}
	//StopAxe();
}

void ALeviathanAxe::StopWeapon()
{
	StopAxe();
	WeaponThrowTraceTimelineComponent->Stop();
}

void ALeviathanAxe::InitVFX()
{
	TopPosition = WeaponMesh->GetSocketLocation(TEXT("TipSocket"));
	BottomPosition = WeaponMesh->GetSocketLocation(TEXT("BladeSocket"));

	FVector CalcPosition = TopPosition - BottomPosition;
	FVector CalcPosition2 = (TopPosition + BottomPosition) / 2.0f;
	FRotator CalcRotator = UKismetMathLibrary::MakeRotFromZ(CalcPosition);

	BladeNiagaraComponent->SetFloatParameter(TEXT("Width"), CalcPosition.Length());
	BladeNiagaraComponent->SetWorldRotation(CalcRotator);
	BladeNiagaraComponent->SetWorldLocation(CalcPosition2);
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
	if (nullptr != BladeNiagaraComponent)
	{
		BladeNiagaraComponent->Activate();
	}
}

void ALeviathanAxe::EndWeaponTrail()
{
	if (nullptr != BladeNiagaraComponent)
	{
		BladeNiagaraComponent->Deactivate();
	}
}

void ALeviathanAxe::CheckHitCollision()
{
	FVector WeaponLocation = GetActorLocation();
	FVector WeaponVelocity = GetVelocity();
	WeaponVelocity.Normalize(0.0001f);
	WeaponVelocity *= ThrowTraceDistance;
	
	FVector StartLocation = WeaponLocation;
	FVector EndLocation = WeaponLocation + WeaponVelocity;

	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility,
		QueryParams
	);

	if (true == bHit)
	{
		bool bBlockingHit = HitResult.bBlockingHit;
		FVector ImpactLocation = HitResult.ImpactPoint;
		FVector ImpactNormal = HitResult.Normal;
		//UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get();
		//EPhysicalSurface SurfaceType = PhysMat->SurfaceType;
		AActor* HitActor = HitResult.GetActor();
		FName HitBoneName = HitResult.BoneName;

		// TODO: End Throw weapon components
		EndWeaponTrail();
		PlayHitSoundCue(ImpactLocation);
		LodgeAxe(ImpactNormal, ImpactLocation);
	}
}

void ALeviathanAxe::PlayHitSoundCue(FVector ImpactLocation)
{
	if (nullptr != HitSoundCue1 && nullptr != HitSoundCue2)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			HitSoundCue1,
			ImpactLocation
		);

		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			HitSoundCue2,
			ImpactLocation
		);
	}
}

void ALeviathanAxe::LodgeAxe(FVector ImpactNormal, FVector ImpactLocation)
{
	StopWeapon();

	PivotPointComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	SetActorRotation(CameraStartRotation);

	FRotator CalcRotator = FRotator(
		CalcAxeImpactPitch(ImpactNormal, FMath::RandRange(-30.0f, -55.0f), FMath::RandRange(-5.0f, -25.0f)),
		0.0f, 
		FMath::RandRange(-3.0f, -8.0f));

	LodgePointComponent->SetRelativeRotation(CalcRotator);
	SetActorLocation(CalcAxeImactLocation(ImpactNormal, ImpactLocation));
	CurWeaponState = ERagnarokWeaponState::ERWS_Lodge;
}

void ALeviathanAxe::StopAxe()
{
	ProjectileMovementComponent->Deactivate();
	WeaponRotTimelineComponent->Stop();
}

float ALeviathanAxe::CalcAxeImpactPitch(FVector ImpactNormal, float InclinedSurfaceRange, float RegularSurfaceRange)
{
	float PitchValue = UKismetMathLibrary::MakeRotationFromAxes(ImpactNormal, FVector::ZeroVector, FVector::ZeroVector).Pitch;
	
	if (PitchValue > 0.0f)
	{
		PitchValue = RegularSurfaceRange - PitchValue;
	}
	else
	{
		PitchValue = InclinedSurfaceRange - PitchValue;
	}
	
	return PitchValue;
}

FVector ALeviathanAxe::CalcAxeImactLocation(FVector ImpactNormal, FVector ImpactLocation)
{
	float PitchValue = UKismetMathLibrary::MakeRotationFromAxes(ImpactNormal, FVector::ZeroVector, FVector::ZeroVector).Pitch;

	if (PitchValue <= 0.0f)
	{
		PitchValue = 0.0f;
	}

	PitchValue = 90.0f - PitchValue;
	PitchValue /= 90.0f;
	PitchValue *= 10.0f;
	ZAdjustment = PitchValue;
	ImpactLocation += FVector(0.0f, 0.0f, PitchValue);
	FVector CalcLocation = GetActorLocation() - LodgePointComponent->GetComponentLocation();
	CalcLocation = CalcLocation + ImpactLocation;

	return CalcLocation;
}

void ALeviathanAxe::WiggleLodgedAxe()
{
	LodgeRotation = LodgePointComponent->GetRelativeRotation();

	WeaponWiggleTimelineComponent->SetPlayRate(3.5f);
	WeaponWiggleTimelineComponent->PlayFromStart();

}

float ALeviathanAxe::GetClampedDistanceFromOwnerCharacter(float MaxDistance)
{
	if (false == OwnerKratos.IsValid())
	{
		Debug::Print(TEXT("ALeviathanAxe::OwnerKratos is not valid"));
		return 0.0f;
	}

	return FMath::Clamp(
		(GetActorLocation() - OwnerKratos->GetMesh()->GetSocketLocation(TEXT("RightWeaponSocket"))).Length(),
		0.0f,
		MaxDistance
	);
}

void ALeviathanAxe::SetWeaponRecallLocation()
{
	SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, ((1.0f - (ZAdjustment / 10.0f)) * 30.0f) + 20.0f));
}

float ALeviathanAxe::CalcRecallTimelinePlayRate(float Distance, float WeaponRecallSpeed)
{
	return FMath::Clamp((Distance * WeaponRecallSpeed) / DistanceFromOwner, 0.4f, 7.0f);
}
