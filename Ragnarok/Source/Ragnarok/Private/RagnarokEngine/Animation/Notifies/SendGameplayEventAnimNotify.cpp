// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/Animation/Notifies/SendGameplayEventAnimNotify.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "RagnarokEngine/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Core/Tools/RagnarokDebugHelper.h"

void USendGameplayEventAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (nullptr == MeshComp)
	{
		Debug::Print(TEXT("USendGameplayEventAnimNotify::Notify's MeshComp is nullptr"), FColor::Red);
		return;
	}

	AActor* Owner = MeshComp->GetOwner();

	if (nullptr == Owner)
	{
		Debug::Print(TEXT("USendGameplayEventAnimNotify::Notify's Owner is nullptr"), FColor::Red);
		return;
	}

	URagnarokAbilitySystemComponent* ASC = Cast<URagnarokAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner));

	if (nullptr == ASC)
	{
		Debug::Print(TEXT("USendGameplayEventAnimNotify::Notify's ASC is nullptr"), FColor::Red);
		return;
	}

	if (true == EventTag.IsValid())
	{
		FGameplayEventData EventData;
		EventData.Instigator = Owner;
		EventData.Target = Owner;

		ASC->HandleGameplayEvent(EventTag, &EventData);
	}
	else
	{
		Debug::Print(TEXT("USendGameplayEventAnimNotify::Notify's EventTag is not valid"), FColor::Red);
		return;
	}
}