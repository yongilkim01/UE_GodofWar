// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/Animation/RagnarokAnimNotify.h"
#include "GameplayTagContainer.h"
#include "SendGameplayEventAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class RAGNAROK_API USendGameplayEventAnimNotify : public URagnarokAnimNotify
{
	GENERATED_BODY()

public:
    virtual void Notify(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        const FAnimNotifyEventReference& EventReference
    ) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ragnarok|Notify")
    FGameplayTag EventTag;
};
