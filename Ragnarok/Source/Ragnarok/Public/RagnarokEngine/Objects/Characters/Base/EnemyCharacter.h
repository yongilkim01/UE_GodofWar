// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"
#include "EnemyCharacter.generated.h"

class UEnemyCombatComponent;
class UEnemyUIComponent;
class UWidgetComponent;

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
	virtual void Die(TSoftObjectPtr<UNiagaraSystem> DeathNiagaraEffect) override;
	//~ End ICombatInterface Interface.

	//~ Begin IUIInterface Interface.
	virtual URagnarokUIComponent* GetUIComponent() const override;
	//~ End IUIInterface Interface.

private:
	void InitEnemyStartUpData();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ragnarok|Enmey")
	UEnemyCombatComponent* EnemyCombatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ragnarok|Enmey")
	UEnemyUIComponent* EnemyUIComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Enemy")
	UWidgetComponent* EnemyWidgetComponent = nullptr;

private:
	FTimerHandle DissolveTimerHandle;
	float DissolveElapsed = 0.0f;
	float DissolveDuration = 2.0f;

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
	
};
