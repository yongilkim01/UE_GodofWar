// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/ControlSystem/Components/ControlComponent.h"
#include "KratosControlComponent.generated.h"

class AKratos;
class AKratosController;
class UInputConfigDataAsset;
class URagnarokEnhancedInputComponent;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosControlComponent : public UControlComponent
{
	GENERATED_BODY()
public:
	friend class AKratos;

public:
	UKratosControlComponent();

	void InitControlComponent();
	URagnarokEnhancedInputComponent* SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

protected:
	//~ Begin UActorComponent Interface.
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ Begin UActorComponent Interface.

private:
	void CalcCharacterRotation(float DeltaTime);
	bool IsMoving() const;
	FRotator GetTargetRotation() const;

	void InputMovePressed(const FInputActionValue& InputActionValue);
	void InputMoveReleased(const FInputActionValue& InputActionValue);
	void InputLook(const FInputActionValue& InputActionValue);
	void InputRun(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Component")
	UInputConfigDataAsset* InputConfigDA = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Component")
	bool bUseControllerRotation = true;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Component")
	float WalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Component")
	float RunSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|Component")
	float InterpolateSpeed = 50.0f;

	UPROPERTY()
	AKratos* Kratos = nullptr;

	UPROPERTY()
	AKratosController* KratosController = nullptr;

	float MovementThreshold = 1.0f;
	bool bRunning = false;
	FVector2D MovementInputVector;
};
