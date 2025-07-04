// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"
#include "EnemyCharacter.generated.h"

class UEnemyCombatComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API AEnemyCharacter : public ARagnarokCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

	//~ Begin ICombatInterface Interface.
	virtual UCombatComponent* GetCombatComponent() const override;
	//~ End ICombatInterface Interface.

private:
	void InitEnemyStartUpData();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ragnarok|Enmey")
	UEnemyCombatComponent* EnemyCombatComponent = nullptr;

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
	
};
