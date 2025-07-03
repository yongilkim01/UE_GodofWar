// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RagnarokWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UItemPrimaryAssetKratosWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponInitialized);

UCLASS()
class RAGNAROK_API ARagnarokWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARagnarokWeapon();

public:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

public:
	virtual void InitWeapon();

protected:
	virtual void LoadWeaponDataAsset();
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject);

	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(
		UPrimitiveComponent*OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Data")
	UItemPrimaryAssetKratosWeapon* WeaponPDA = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Data")
	FPrimaryAssetId WeaponPDAId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Components")
	UBoxComponent* WeaponCollision = nullptr;

	bool bInitialized = false;

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
	FORCEINLINE bool IsInitialize() const { return bInitialized; }

	UPROPERTY(BlueprintAssignable, Category = "Weapon|Delegate")
	FOnWeaponInitialized OnWeaponInitialized;
};
