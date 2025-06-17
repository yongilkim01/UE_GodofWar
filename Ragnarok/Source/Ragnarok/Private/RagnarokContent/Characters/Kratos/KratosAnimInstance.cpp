// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/KratosAnimInstance.h"

#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"

void UKratosAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (nullptr != OwnerCharacter)
	{
		Kratos = Cast<AKratos>(OwnerCharacter);
	
		if (nullptr == Kratos)
		{
			Debug::Print(TEXT("Kratos is nullptr check owner character type"), FColor::Red);
		}
	}
	
	IdleElapsedTime = 0.0f;
}

void UKratosAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (true == bAcceleration)
	{
		bRelax = false;
		IdleElapsedTime = 0.0f;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;

		bRelax = (IdleElapsedTime >= RelaxStateThreshold);
	}
}
