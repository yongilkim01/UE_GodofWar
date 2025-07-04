// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/Items/Weapons/RagnarokWeapon.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "KratosWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API AKratosWeapon : public ARagnarokWeapon
{
	GENERATED_BODY()

public:
	virtual void InitWeapon() override;

	void AssignGratnAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandleArray);
	TArray<FGameplayAbilitySpecHandle>& GetGrantedAbilitySpecHandleArray();


protected:
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	//~ Begin ARagnarokWeapon Interface.
	virtual void LoadWeaponDataAsset() override;
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject) override;
	//~ End ARagnarokWeapon Interface.

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandleArray;

protected:
	UItemPrimaryAssetKratosWeapon* KratosWeaponPDA = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|WeaponData")
	FKratosWeaponData WeaponData;
};
