// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RagnarokUserWidget.generated.h"

class UKratosUIComponent;

/**
 * 
 */
UCLASS()
class RAGNAROK_API URagnarokUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnKratosUIComponentInitialized(UKratosUIComponent* KratosUIComponent);

	//UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Kratos UI Component Initialize"))
	//void BP_OnKratosUIComponentInitialized(UKratosUIComponent* KratosUIComponent);

public:
	UFUNCTION()
	void TestBind(float CurrentHealth);
};
