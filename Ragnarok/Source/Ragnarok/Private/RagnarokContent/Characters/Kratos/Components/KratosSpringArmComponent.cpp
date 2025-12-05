// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/Components/KratosSpringArmComponent.h"
#include "RagnarokContent/Characters/Kratos/Kratos.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"

#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"
#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"
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
		ERagnarokCameraMode NewCameraMode = ERagnarokCameraMode::ERCM_None;

		if (true == URagnarokFunctionLibrary::HasActorGameplayTag(OwnerKratos, KratosGameplayTags::Kratos_Status_Attacking))
		{
			NewCameraMode = ERagnarokCameraMode::EPCM_Combat;
		}
		else if (true == URagnarokFunctionLibrary::HasActorGameplayTag(OwnerKratos, KratosGameplayTags::Kratos_Status_Running))
		{
			NewCameraMode = ERagnarokCameraMode::EPCM_Running;
		}
		else
		{
			NewCameraMode = ERagnarokCameraMode::ERCM_None;
		}

		if (NewCameraMode != CurrentCameraMode)
		{
			SetCameraMode(NewCameraMode);
		}
	}

	InterpolateCameraSettings(DeltaTime);

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
