// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/GameFramework/RagnarokCharacter.h"

#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokAttributeSet.h"

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
	}
}

UAbilitySystemComponent* ARagnarokCharacter::GetAbilitySystemComponent() const
{
	return GetAbilitySystem();
}

