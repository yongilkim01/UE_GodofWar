// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Abilities/Notifies/SendGameplayEventAnimNotify.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "RagnarokEngine/Core/GameplayAbilities/RagnarokAbilitySystemComponent.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

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

	if (true == EventTag.IsValid())
	{
		FGameplayEventData EventData;
		EventData.Instigator = Owner;
		EventData.Target = Owner;

		// ��������Ʈ�� "Send Gameplay Event To Actor" ���� ������ ���
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, EventData);
	}
	else
	{
		Debug::Print(TEXT("USendGameplayEventAnimNotify::Notify's EventTag is not valid"), FColor::Red);
		return;
	}
}