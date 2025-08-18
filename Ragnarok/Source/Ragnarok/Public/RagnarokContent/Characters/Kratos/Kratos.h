// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "GameplayTagContainer.h"
#include "Kratos.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInitDataAssetKratos;
class UCharacterPrimaryAssetKratos;
class UInputConfigDataAsset;
class UInputComponent;
class UKratosCombatComponent;
class UKratosUIComponent;
class UKratosGameplayAbility;
class UKratosControlComponent;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class RAGNAROK_API AKratos : public ARagnarokCharacter
{
		GENERATED_BODY()
public:
	friend class UKratosGameplayAbility;
	friend class UKratosControlComponent;

public:
	AKratos();

	//~ Begin IUIInterface Interface.
	virtual URagnarokUIComponent* GetUIComponent() const override;
	//~ End IUIInterface Interface.
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//~ Begin ICombatInterface Interface.
	virtual UCombatComponent* GetCombatComponent() const override;
	//~ End ICombatInterface Interface.

private:
	void LoadKratosDataAsset();
	void InitPrimaryData(UObject* PDAObject);

	void InputAbilityPressed(FGameplayTag InputTag);
	void InputAbilityReleased(FGameplayTag InputTag);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Combat", meta = (AllowPrivateAccess = "true"))
	UKratosCombatComponent* KratosCombatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Combat", meta = (AllowPrivateAccess = "true"))
	UKratosUIComponent* KratosUIComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Combat", meta = (AllowPrivateAccess = "true"))
	UKratosControlComponent* KratosControlComponent = nullptr;

private:
	UCharacterPrimaryAssetKratos* CharacterPDA = nullptr;
	bool bRolling = false;
	bool bAttacking = false;
	float FixedCameraWorldZLocation = 0.0f;
	FTimerHandle RunningTimerHandle;

public:
	FORCEINLINE UKratosCombatComponent* GetKratosCombatComponent() const { return KratosCombatComponent; }
	FORCEINLINE bool IsAttacking() const { return bAttacking; }
	FORCEINLINE bool IsRolling() const { return bRolling; }
	bool IsRunning() const;
	FVector2D GetMovementInputVector() const;
};
