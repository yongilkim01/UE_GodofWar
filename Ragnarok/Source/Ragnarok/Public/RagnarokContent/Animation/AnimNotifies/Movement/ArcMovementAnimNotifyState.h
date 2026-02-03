// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Core/Animation/RagnarokAnimNotifyState.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
#include "ArcMovementAnimNotifyState.generated.h"

class ARagnarokCharacter;

/**
 * 
 */
UCLASS()
class RAGNAROK_API UArcMovementAnimNotifyState : public URagnarokAnimNotifyState
{
	GENERATED_BODY()

public:
	UArcMovementAnimNotifyState();

	//~ Begin UAnimNotifyState Interface.
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//~ End UAnimNotifyState Interface.


private:
	FVector CalcArcLocation(float Alpha) const;
	FVector CalcParablicLocation(float Alpha) const;

private:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Animation")
	EArcMovementType ArcMovementType = EArcMovementType::EAMT_Parabolic;
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Animation", meta = (EditCondition = "ArcMovementType == EArcMovementType::EAMT_Custom"))
	TArray<FVector> CustomArcPointArray;
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Animation", meta = (ClampMin = 0.0f))
	float MovementDistance = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Animation", meta = (ClampMin = 0.0f))
	float ArcHeight = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Animation", meta = (ClampMin = 0.0f))
	float Duration = 0.8f;
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Animation")
	UCurveFloat* ArcMovementCurve = nullptr;
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Animation")
	bool bMoveWorldSpace = false;

private:
	FVector StartLocation = FVector::ZeroVector;
	FVector TargetLocation = FVector::ZeroVector;
	FVector ForwardDirection = FVector::ZeroVector;
	float ElapsedTime = 0.0f;

	UPROPERTY()
	ARagnarokCharacter* RagnarokCharacter = nullptr;
};
