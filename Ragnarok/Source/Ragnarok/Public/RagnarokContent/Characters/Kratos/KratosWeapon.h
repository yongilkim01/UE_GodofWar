// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Objects/Items/Weapons/RagnarokWeapon.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "KratosWeapon.generated.h"

class UProjectileMovementComponent;
class UTimelineComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API AKratosWeapon : public ARagnarokWeapon
{
	GENERATED_BODY()

public:
	AKratosWeapon();

	virtual void InitWeapon() override;

	void AssignGratnAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandleArray);
	TArray<FGameplayAbilitySpecHandle>& GetGrantedAbilitySpecHandleArray();
	
	virtual void ThrowWeapon(
		FRotator CameraRotation, 
		FVector CameraLocation, 
		FVector CameraForwardVector) {}

	virtual void ThrowWeaponToTarget(
		FVector StartLocation,
		FVector TargetLocation) {}

	void UnUequipWeapon(const AActor* OwnerActor, FName SocketName);

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	//~ Begin ARagnarokWeapon Interface.
	virtual void LoadWeaponDataAsset() override;
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject) override;
	//~ End ARagnarokWeapon Interface.

private:
	void UnlinkAnimClassLayersFromActor(const AActor* UnlinkActor);

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandleArray;

protected:
	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UItemPrimaryAssetKratosWeapon* KratosWeaponPDA = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|WeaponData")
	FKratosWeaponData WeaponData;
};
