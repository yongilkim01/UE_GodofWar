// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/Animation/RagnarokAnimNotifyState.h"
#include "KratosCatchWeaponAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosCatchWeaponAnimNotifyState : public URagnarokAnimNotifyState
{
	GENERATED_BODY()

public:
	UKratosCatchWeaponAnimNotifyState();

protected:
	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//~ End UAnimNotifyState Interface.
	
};
