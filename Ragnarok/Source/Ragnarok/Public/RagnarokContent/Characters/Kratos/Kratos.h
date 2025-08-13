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

	void InputMovePressed(const FInputActionValue& InputActionValue);
	void InputMoveReleased(const FInputActionValue& InputActionValue);
	void InputLook(const FInputActionValue& InputActionValue);
	void InputRun(const FInputActionValue& InputActionValue);
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

private:
	UCharacterPrimaryAssetKratos* CharacterPDA = nullptr;
	FVector2D CachedMovementInputVector;
	int AttackCount = 0;
	bool bRolling = false;
	bool bAttacking = false;
	bool bRunning = false;
	float RunSpeed = 0.0f;
	float WalkSpeed = 0.0f;
	float FixedCameraWorldZLocation = 0.0f;
	FTimerHandle RunningTimerHandle;

public:
	FORCEINLINE UKratosCombatComponent* GetKratosCombatComponent() const { return KratosCombatComponent; }
	FORCEINLINE int GetKratosAttackCount() const { return AttackCount; }
	FORCEINLINE void SetKratosAttackCount(int Count) { AttackCount = Count; }
	FORCEINLINE void AddKratosAttackCount(int Count) { AttackCount += Count; }
	FORCEINLINE FVector2D GetCachedMovementInputVector() { return CachedMovementInputVector; }
};
