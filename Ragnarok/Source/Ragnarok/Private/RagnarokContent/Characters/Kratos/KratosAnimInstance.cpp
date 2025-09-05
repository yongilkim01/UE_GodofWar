// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/KratosAnimInstance.h"

#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"
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

	//Debug::Print(TEXT("Kratos is nullptr check owner character type"), FColor::Red);

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

	//FString DebugMessage = FString::Printf(TEXT("ForwardSpeed: %.2f, RightSpeed: %.2f"),
	//	ForwardSpeed, RightSpeed);
	//Debug::Print(DebugMessage, FColor::Green, false, 1, 2.0f);
}