// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokGameplayCueNotify_Static.h"
#include "SoundPlayGameplayCueNotify.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API USoundPlayGameplayCueNotify : public URagnarokGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|CueNotify")
	USoundBase* SoundToPlay = nullptr;
};
