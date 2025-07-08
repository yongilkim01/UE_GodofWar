// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"

#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAttributeSet.h"

ARagnarokCharacter::ARagnarokCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	AbilitySystemComponent = CreateDefaultSubobject<URagnarokAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<URagnarokAttributeSet>(TEXT("AttributeSet"));
}

void ARagnarokCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARagnarokCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARagnarokCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARagnarokCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (nullptr != AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(false == StartUpData.IsNull(), TEXT("ARagnarokCharacter::StartUpData is not ensure. Name : %s"), *GetName());
	}
}

UAbilitySystemComponent* ARagnarokCharacter::GetAbilitySystemComponent() const
{
	return GetAbilitySystem();
}

UCombatComponent* ARagnarokCharacter::GetCombatComponent() const
{
	return nullptr;
}

void ARagnarokCharacter::Die(TSoftObjectPtr<UNiagaraSystem> DeathNiagaraEffect)
{
}

URagnarokUIComponent* ARagnarokCharacter::GetUIComponent() const
{
	return nullptr;
}