// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RagnarokCharacter.generated.h"

class URagnarokAbilitySystemComponent;
class URagnarokAttributeSet;

UCLASS()
class RAGNAROK_API ARagnarokCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARagnarokCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface.

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	URagnarokAbilitySystemComponent* AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|AttributeSets")
	URagnarokAttributeSet* AttributeSet = nullptr;

public:
	FORCEINLINE URagnarokAbilitySystemComponent* GetAbilitySystem() const { return AbilitySystemComponent; }
	FORCEINLINE URagnarokAttributeSet* GetAttributeSet() const { return AttributeSet; }
};
