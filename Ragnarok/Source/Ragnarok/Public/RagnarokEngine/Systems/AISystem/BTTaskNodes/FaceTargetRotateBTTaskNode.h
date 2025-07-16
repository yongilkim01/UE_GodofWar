// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Systems/AISystem/RagnarokBTTaskNode.h"
#include "FaceTargetRotateBTTaskNode.generated.h"

struct FFaceTargetRotateTaskMemory
{
	TWeakObjectPtr<APawn> OwnerPawn;
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid() const
	{
		return true == OwnerPawn.IsValid() && true == TargetActor.IsValid();
	}

	void Reset()
	{
		OwnerPawn.Reset();
		TargetActor.Reset();
	}
};

/**
 * 
 */
UCLASS()
class RAGNAROK_API UFaceTargetRotateBTTaskNode : public URagnarokBTTaskNode
{
	GENERATED_BODY()

	UFaceTargetRotateBTTaskNode();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	UPROPERTY(EditAnywhere, Category = "AI|FaceTarget")
	float AnglePrecision;

	UPROPERTY(EditAnywhere, Category = "AI|FaceTarget")
	float RotationInterpolateSpeed;

	UPROPERTY(EditAnywhere, Category = "AI|FaceTarget")
	FBlackboardKeySelector TargetToFaceKey;
};
