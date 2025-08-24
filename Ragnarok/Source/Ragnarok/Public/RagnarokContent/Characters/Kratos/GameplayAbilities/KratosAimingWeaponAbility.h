// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosGameplayAbility.h"
#include "KratosAimingWeaponAbility.generated.h"

class AKratosWeapon;
class URagnarokUserWidget;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosAimingWeaponAbility : public UKratosGameplayAbility
{
	GENERATED_BODY()
	
public:
	UKratosAimingWeaponAbility();

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~ End UGameplayAbility Interface.

private:
	void StartAiming();
	void EndAiming();
	void ThrowWeapon();

private:
	UPROPERTY(EditAnywhere, Category = "Ragnarok")
	UAnimMontage* AimAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok")
	UAnimMontage* ThrowAnimMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ragnarok")
	TSubclassOf<URagnarokUserWidget> AimWidgetClass;

	UPROPERTY()
	AKratosWeapon* CurWeapon = nullptr;

	UPROPERTY()
	URagnarokUserWidget* AimWidget = nullptr;

	bool bAiming = false;
	float AimCameraDistance = 100.0f;
	float AimCameraInterpSpeed = 5.0f;

};
