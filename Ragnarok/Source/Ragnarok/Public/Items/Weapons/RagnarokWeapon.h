// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RagnarokWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class RAGNAROK_API ARagnarokWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARagnarokWeapon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponData")
	UBoxComponent* WeaponCollision = nullptr;

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
};
