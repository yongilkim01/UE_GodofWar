// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/Animation/RagnarokAnimNotifyState.h"
#include "RagnarokEngine/Core/Types/RagnarokTypes.h"
#include "WeaponCollisionAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API UWeaponCollisionAnimNotifyState : public URagnarokAnimNotifyState
{
	GENERATED_BODY()

public:	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

protected:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Weapon")
	EToggleDamageType ToggleWeaponType = EToggleDamageType::ETD_CurrentEquippedWeapon;
	
};
