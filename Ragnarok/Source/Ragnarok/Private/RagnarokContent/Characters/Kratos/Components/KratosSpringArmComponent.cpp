// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Components/KratosSpringArmComponent.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"
#include "RagnarokEngine/Systems/AbilitySystem/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Kismet/RagnarokFunctionLibrary.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

UKratosSpringArmComponent::UKratosSpringArmComponent()
{
}

void UKratosSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	AKratos* OwnerKratos = Cast<AKratos>(GetOwner());

	if (nullptr != OwnerKratos)
	{
		URagnarokAbilitySystemComponent* ASC = OwnerKratos->GetAbilitySystem();

		if (true == URagnarokFunctionLibrary::HasActorGameplayTag(OwnerKratos, KratosGameplayTags::Kratos_Status_Attacking))
		{
			Debug::Print(TEXT("Current Kratos Has Kratos Status Attacking"));
			SetCameraMode(ERagnarokCameraMode::EPCM_Combat);
		}
		else if (true == OwnerKratos->IsRunning())
		{
			Debug::Print(TEXT("Current Kratos Has Kratos Status Running"));
			SetCameraMode(ERagnarokCameraMode::EPCM_Running);
		}
		else
		{
			Debug::Print(TEXT("Current Kratos Has None"));
			SetCameraMode(ERagnarokCameraMode::ERCM_None);
		}
	}

	InterpolateCameraSettings(DeltaTime);

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
