// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Systems/UISystem/RagnarokUserWidget.h"
#include "RagnarokEngine/Systems/UISystem/UIInterface.h"

#include "RagnarokContent/Characters/Kratos/Components/KratosUIComponent.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"
#include "RagnarokEngine/Systems/EnemySystem/EnemyUIComponent.h"

void URagnarokUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IUIInterface* UIInterface = Cast<IUIInterface>(GetOwningPlayerPawn()))
	{
		if (UKratosUIComponent* KratosUIComponent = Cast<UKratosUIComponent>(UIInterface->GetUIComponent()))
		{
			BP_OnKratosUIComponentInitialized(KratosUIComponent);
		}
	}
}

//void URagnarokUserWidget::OnKratosUIComponentInitialized(UKratosUIComponent* KratosUIComponent)
//{
//	//KratosUIComponent->OnCurrentHealthChanged.AddDynamic(this, &URagnarokUserWidget::TestBind);
//}
//
//void URagnarokUserWidget::OnEnemyUIComponentInitialized(UEnemyUIComponent* EnemyUIComponent)
//{
//	//EnemyUIComponent->OnCurrentHealthChanged.AddDynamic(this, &URagnarokUserWidget::TestBind);
//}

void URagnarokUserWidget::InitEnemyCreatedWidet(AActor* EnemyActor)
{
	if (IUIInterface* UIInterface = Cast<IUIInterface>(EnemyActor))
	{
		if (UEnemyUIComponent* EnemyUIComponent = Cast<UEnemyUIComponent>(UIInterface->GetUIComponent()))
		{
			BP_OnEnemyUIComponentInitialized(EnemyUIComponent);
		}
	}
}
