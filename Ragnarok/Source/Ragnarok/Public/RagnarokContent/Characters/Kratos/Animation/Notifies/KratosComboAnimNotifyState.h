// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/Animation/RagnarokAnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "KratosComboAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UKratosComboAnimNotifyState : public URagnarokAnimNotifyState
{
	GENERATED_BODY()

public:
	UKratosComboAnimNotifyState();

	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//~ End UAnimNotifyState Interface.

private:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|ANS")
	FGameplayTag AttackWaitStartEventTag;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|ANS")
	FGameplayTag AttackWaitEndEventTag;

	UPROPERTY(EditAnywhere, Category = "Ragnarok|ANS")
	int32 ComboIndex = 1;
};
