// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KratosConfigDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsConfig")
    FVector SkeletalMeshOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsConfig")
    float CapsuleRadius = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsConfig")
    float CapsuleHalfHeight = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsConfig")
    float TargetArmLength = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsConfig")
    FVector SocketOffset = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsConfig")
    FRotator CharacterMovementRotationRate = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsConfig")
    float MaxWalkSpeed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ComponentsConfig")
    float BrakingDecelerationWalking = 0.0f;
};
