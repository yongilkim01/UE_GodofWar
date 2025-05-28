// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RagnarokCharacter.h"

ARagnarokCharacter::ARagnarokCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

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

