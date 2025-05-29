// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RagnarokCharacter.h"
#include "Kratos.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UKratosConfigDataAsset;
class UKratosConfigPrimaryDataAsset;

/**
 * 
 */
UCLASS()
class RAGNAROK_API AKratos : public ARagnarokCharacter
{
	GENERATED_BODY()

public:
	AKratos();
	
protected:
	virtual void BeginPlay() override;

private:
	void LoadKratosConfigData();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	UKratosConfigDataAsset* KratosConfig = nullptr;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* MainCameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent = nullptr;

private:
	FPrimaryAssetId KratosConfigPrimaryAssetId;
	UKratosConfigPrimaryDataAsset* KratosConfigPrimaryDataAsset = nullptr;
};
