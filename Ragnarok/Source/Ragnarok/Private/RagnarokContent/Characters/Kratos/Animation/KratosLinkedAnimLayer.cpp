// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Animation/KratosLinkedAnimLayer.h"

#include "RagnarokContent/Characters/Kratos/KratosAnimInstance.h"

UKratosAnimInstance* UKratosLinkedAnimLayer::GetKratosAnimInstance() const
{
    return Cast<UKratosAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
