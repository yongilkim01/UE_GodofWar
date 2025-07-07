// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Kratos/GameplayCues/KratoAxeHitGameplayCueNotify.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

bool UKratoAxeHitGameplayCueNotify::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
    if (nullptr != SoundToPlay)
    {
        UGameplayStatics::PlaySoundAtLocation(
            MyTarget->GetWorld(),
            SoundToPlay,
            MyTarget->GetActorLocation()
        );
    }
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
