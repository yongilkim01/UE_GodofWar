// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/GameItem/Weapon/RagnarokWeapon.h"
#include "RagnarokEngine/AssetSystem/RagnarokAssetManager.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"


ARagnarokWeapon::ARagnarokWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(GetRootComponent());
	WeaponCollision->SetBoxExtent(FVector(20.0f));
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}

void ARagnarokWeapon::BeginPlay()
{
	Super::BeginPlay();

	LoadWeaponDataAsset();
	//LoadWeaponPrimaryDataAsset();
}

void ARagnarokWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARagnarokWeapon::InitWeapon()
{
}

void ARagnarokWeapon::LoadWeaponDataAsset()
{
}

void ARagnarokWeapon::LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject)
{

}

void ARagnarokWeapon::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn of the weapon : %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			Debug::Print(GetName() + TEXT(" begin overlap with ") + HitPawn->GetName(), FColor::Green);
		}
	}
}

void ARagnarokWeapon::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forgot to assign an instigator as the owning pawn of the weapon : %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			Debug::Print(GetName() + TEXT(" end overlap with ") + HitPawn->GetName(), FColor::Orange);
		}
	}
}
