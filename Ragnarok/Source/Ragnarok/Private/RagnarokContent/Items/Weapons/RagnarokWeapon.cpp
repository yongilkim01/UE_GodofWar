// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Items/Weapons/RagnarokWeapon.h"
#include "RagnarokEngine/Asset/RagnarokAssetManager.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"

#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"


ARagnarokWeapon::ARagnarokWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComponent);

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

void ARagnarokWeapon::AttachWeaponToActor(const AActor* ActorToAttach, const FAttachmentTransformRules& AttachmentRules, FName SocketName)
{
	if (nullptr == ActorToAttach)
	{
		Debug::Print(TEXT("ARagnarokWeapon::AttachWeaponToActor method's ActorToAttach is nullptr"), FColor::Red);
		return;
	}

	USkeletalMeshComponent* ParentMesh = ActorToAttach->FindComponentByClass<USkeletalMeshComponent>();

	if (nullptr != ParentMesh)
	{
		AttachToComponent(ParentMesh, AttachmentRules, SocketName);
	}
}

void ARagnarokWeapon::LoadWeaponDataAsset()
{
}

void ARagnarokWeapon::LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject)
{

}

void ARagnarokWeapon::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwner = GetInstigator<APawn>();

	checkf(WeaponOwner, TEXT("ARagnarokWeapon::OnCollisionBoxBeginOverlap WeaponOwner is nullptr - %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (true == URagnarokFunctionLibrary::IsTargetPawnHostile(WeaponOwner, HitPawn))
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}

void ARagnarokWeapon::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwner = GetInstigator<APawn>();

	checkf(WeaponOwner, TEXT("ARagnarokWeapon::OnCollisionBoxBeginOverlap WeaponOwner is nullptr - %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (true == URagnarokFunctionLibrary::IsTargetPawnHostile(WeaponOwner, HitPawn))
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}
	}
}
