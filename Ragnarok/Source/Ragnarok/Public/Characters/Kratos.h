// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RagnarokCharacter.h"
#include "Core/Types/RagnarokTypes.h"
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
	
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.

private:
	void LoadKratosDataAsset();
	void LoadKratosPrimaryDataAsset(UObject* PDAAssetObject);

	void InputMove(const FInputActionValue& InputActionValue);
	void InputLook(const FInputActionValue& InputActionValue);

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
	//FPrimaryAssetId CharacterPDAId;
};
