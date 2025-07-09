// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayAbilities/KratosDrawOverlayWidgetAbility.h"
#include "RagnarokContent/Characters/Kratos/KratosController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UKratosDrawOverlayWidgetAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentActorInfo = ActorInfo;

	if (nullptr != WidgetClass)
	{
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetKratosControllerFromActorInfo(), WidgetClass);

		if (nullptr != WidgetInstance)
		{
			WidgetInstance->AddToViewport();
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
