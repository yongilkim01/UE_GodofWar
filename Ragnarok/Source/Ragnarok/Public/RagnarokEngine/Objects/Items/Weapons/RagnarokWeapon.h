// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
#include "RagnarokWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UItemPrimaryAssetKratosWeapon;
class ARagnarokCharacter;

struct FAttachmentTransformRules;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponInitialized);
DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*);

UCLASS()
class RAGNAROK_API ARagnarokWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARagnarokWeapon();

public:
	virtual void InitWeapon() {}
	virtual void SetOwner(ARagnarokCharacter* RagnarokCharacter) {};

	void AttachWeaponToActor(const AActor* ActorToAttach, const FAttachmentTransformRules& AttachmentRules, FName SocketName);

protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

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
	UPROPERTY(EditAnywhere)
	USceneComponent* RootSceneComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UItemPrimaryAssetKratosWeapon* WeaponPDA = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId WeaponPDAId;

protected:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere)
	UBoxComponent* WeaponCollision = nullptr;

	bool bInitialized = false;
	ERagnarokWeaponState CurWeaponState = ERagnarokWeaponState::ERWS_None;

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
	FORCEINLINE bool IsInitialize() const { return bInitialized; }
	FORCEINLINE ERagnarokWeaponState GetWeaponState() const { return CurWeaponState; }

	UPROPERTY(BlueprintAssignable)
	FOnWeaponInitialized OnWeaponInitialized;

	FOnTargetInteractedDelegate OnWeaponHitTarget;
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;
};
