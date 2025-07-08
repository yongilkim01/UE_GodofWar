// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/UISystem/RagnarokUserWidget.h"
#include "RagnarokEngine/Systems/UISystem/UIInterface.h"

#include "RagnarokContent/Characters/Kratos/Components/KratosUIComponent.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

void URagnarokUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IUIInterface* UIInterface = Cast<IUIInterface>(GetOwningPlayerPawn()))
	{
		if (UKratosUIComponent* KratosUIComponent = Cast<UKratosUIComponent>(UIInterface->GetUIComponent()))
		{
			OnKratosUIComponentInitialized(KratosUIComponent);
		}
	}
}

void URagnarokUserWidget::OnKratosUIComponentInitialized(UKratosUIComponent* KratosUIComponent)
{
	KratosUIComponent->OnCurrentHealthChanged.AddDynamic(this, &URagnarokUserWidget::TestBind);
}

void URagnarokUserWidget::TestBind(float CurrentHealth)
{
	Debug::Print(TEXT("Test Bind"));
}
