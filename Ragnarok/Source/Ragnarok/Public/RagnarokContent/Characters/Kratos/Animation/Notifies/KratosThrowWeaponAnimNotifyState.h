// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/Animation/RagnarokAnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "KratosThrowWeaponAnimNotifyState.generated.h"

class USoundCue;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosThrowWeaponAnimNotifyState : public URagnarokAnimNotifyState
{
	GENERATED_BODY()

public:
	UKratosThrowWeaponAnimNotifyState();

protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//~ End UAnimNotifyState Interface.

private:
	UPROPERTY(EditAnywhere)
	USoundCue* ThrowSoundCue = nullptr;

	FGameplayTag ActiveThrowTag;
};
