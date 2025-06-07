// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RagnarokCharacter.h"
#include "Kratos.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInitDataAssetKratos;
class UCharacterPrimaryAssetKratos;
class UInputConfigDataAsset;
class UInputComponent;

struct FInputActionValue;

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
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void AsyncLoadCharacterKratos();

	void LoadKratosDataAsset();
	void LoadKratosPrimaryDataAsset();

	void InputMove(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Init")
	UInitDataAssetKratos* InitDA = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Input")
	UInputConfigDataAsset* InputConfigDA = nullptr;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* MainCameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent = nullptr;

private:
	UCharacterPrimaryAssetKratos* CharacterPDA = nullptr;
	FPrimaryAssetId CharacterPDAId;
};
