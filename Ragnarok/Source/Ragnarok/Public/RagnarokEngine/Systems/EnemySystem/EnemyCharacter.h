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
	virtual void Die() override;
	//~ End ICombatInterface Interface.

private:
	void InitEnemyStartUpData();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ragnarok|Enmey")
	UEnemyCombatComponent* EnemyCombatComponent = nullptr;

private:
	FTimerHandle DissolveTimerHandle;
	float DissolveElapsed = 0.0f;
	float DissolveDuration = 1.0f;

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
	
};
