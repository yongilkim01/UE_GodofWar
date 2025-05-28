// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RagnarokCharacter.generated.h"

UCLASS()
class RAGNAROK_API ARagnarokCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARagnarokCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
