// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/GameplayAbilities/EnemyHitReactGameplayAbility.h"
#include "RagnarokContent/Characters/Enemy/Base/EnemyCharacter.h"
#include "RagnarokContent/Characters/Kratos/Tags/KratosGameplayTags.h"
#include "RagnarokContent/GameplayAbilities/RagnarokGameplayEffectContext.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"

#include "RagnarokEngine/Core/Tags/RagnarokGameplayTags.h"
#include "RagnarokEngine/Core/GameFramework/RagnarokCharacter.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyHitReactGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (nullptr == TriggerEventData)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    const FRagnarokGameplayEffectContext* Context = static_cast<const FRagnarokGameplayEffectContext*>(TriggerEventData->ContextHandle.Get());
    if (nullptr == Context)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    FGameplayTag ReceivedAttackType = Context->GetAttackTypeTag();
    FVector ReceivedHitDirection = Context->GetHitDirection();

    bool bHandleAttackReact = false;
    if (true == ReceivedAttackType.IsValid())
    {
        if (ReceivedAttackType == KratosGameplayTags::Kratos_AttackReact_Knockback)
        {
            HandleKnockback(ReceivedHitDirection);
            //HandleKnockback(TriggerEventData->Instigator.Get());
            bHandleAttackReact = true;
        }
        else if (ReceivedAttackType == KratosGameplayTags::Kratos_AttackReact_Launch)
        {
            HandleLaunch();
            bHandleAttackReact = true;
        }
        else  if (ReceivedAttackType == KratosGameplayTags::Kratos_AttackReact_Slamdown)
        {
            bHandleAttackReact = HandleSlamdown();
        }
    }

    FGameplayTag EventTag = TriggerEventData->EventTag;
    float EventMagnitude = TriggerEventData->EventMagnitude;

    if (true == bRotateToATargetActor)
    {
        FVector AttackerLocation = TriggerEventData->Instigator.Get()->GetActorLocation();
        FVector EnemyLocation = GetEnemyCharacterFromActorInfo()->GetActorLocation();
        FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, AttackerLocation);

        GetEnemyCharacterFromActorInfo()->SetActorRotation(LookAtRotation);
    }

    if (true == bHasHitReactMontagesToPlay)
    {
        int32 RandomIndex = FMath::RandRange(0, AnimMontageArray.Num() - 1);

        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,
            NAME_None,
            AnimMontageArray[RandomIndex],
            1.0f,
            NAME_None,
            false,
            0.0f,
            false
        );

        USkeletalMeshComponent* ParentMesh = nullptr;

        if (true == CurrentActorInfo->AvatarActor.IsValid())
        {
            APawn* Pawn = Cast<APawn>(CurrentActorInfo->AvatarActor.Get());

            if (nullptr != Pawn)
            {
                ParentMesh = Pawn->FindComponentByClass<USkeletalMeshComponent>();
            }
        }

        ParentMesh->SetScalarParameterValueOnMaterials(TEXT("HitFxSwitch"), 1.0f);

        if (nullptr != MontageTask)
        {
            MontageTask->OnCompleted.AddDynamic(this, &UEnemyHitReactGameplayAbility::OnMontageCompleted);
            MontageTask->OnBlendOut.AddDynamic(this, &UEnemyHitReactGameplayAbility::OnMontageBlendOut);
            MontageTask->OnInterrupted.AddDynamic(this, &UEnemyHitReactGameplayAbility::OnMontageInterrupted);
            MontageTask->OnCancelled.AddDynamic(this, &UEnemyHitReactGameplayAbility::OnMontageCancelled);
            MontageTask->ReadyForActivation();
        }
        else
        {
            EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        }
    }
    else
    {

        USkeletalMeshComponent* ParentMesh = nullptr;

        if (true == CurrentActorInfo->AvatarActor.IsValid())
        {
            APawn* Pawn = Cast<APawn>(CurrentActorInfo->AvatarActor.Get());

            if (nullptr != Pawn)
            {
                ParentMesh = Pawn->FindComponentByClass<USkeletalMeshComponent>();
            }
        }

        ParentMesh->SetScalarParameterValueOnMaterials(TEXT("HitFxSwitch"), 1.0f);

        FTimerHandle TimerHandle;

        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            [this]()
            {
                EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
            },
            2.0f,
            false
        );
    }

    FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(HitEffectClass);
    ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, EffectSpecHandle);
}


void UEnemyHitReactGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    USkeletalMeshComponent* ParentMesh = nullptr;

    if (true == CurrentActorInfo->AvatarActor.IsValid())
    {
        APawn* Pawn = Cast<APawn>(CurrentActorInfo->AvatarActor.Get());

        if (nullptr != Pawn)
        {
            ParentMesh = Pawn->FindComponentByClass<USkeletalMeshComponent>();
            ParentMesh->SetScalarParameterValueOnMaterials(TEXT("HitFxSwitch"), 0.0f);

        }
    }

}

void UEnemyHitReactGameplayAbility::HandleKnockback(const FVector& HitDirection)
{
    AEnemyCharacter* OwnerEnemyCharacter = GetEnemyCharacterFromActorInfo();
    if (nullptr == OwnerEnemyCharacter)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }

    // 1. HitDirection의 Z값을 0으로 만들어 수평 방향 벡터를 구합니다.
    FVector HorizontalHitDirection = HitDirection;
    HorizontalHitDirection.Z = 0.0f;
    
    // 2. 수평 방향으로만 넉백을 적용합니다.
    FVector LaunchVelocity = HorizontalHitDirection.GetSafeNormal() * KnockbackPower;

    // 3. 캐릭터를 살짝 띄우기 위해 고정된 Z값을 더해줍니다. (이 값을 조절하여 뜨는 높이를 제어할 수 있습니다)
    LaunchVelocity.Z = 200.0f;

    UE_LOG(LogTemp, Warning, TEXT("HandleKnockback Called! Horizontal LaunchVelocity: %s"), *LaunchVelocity.ToString());

    OwnerEnemyCharacter->LaunchCharacter(LaunchVelocity, true, true);

}

void UEnemyHitReactGameplayAbility::HandleKnockback(const AActor* Attacker)
{
    AEnemyCharacter* OwnerEnemyCharacter = GetEnemyCharacterFromActorInfo();
    if (nullptr == OwnerEnemyCharacter || nullptr == Attacker)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }

    // 1. 적의 위치와 공격자의 위치를 가져옵니다.
    const FVector EnemyLocation = OwnerEnemyCharacter->GetActorLocation();
    const FVector AttackerLocation = Attacker->GetActorLocation();

    // 2. 공격자로부터 적을 향하는 방향 벡터를 계산합니다.
    FVector KnockbackDirection = EnemyLocation - AttackerLocation;

    // 3. Z값을 0으로 만들어 순수한 수평 방향으로 만듭니다.
    KnockbackDirection.Z = 0.0f;

    // 4. 방향 벡터를 정규화하고 KnockbackPower를 곱해 최종 힘을 계산합니다.
    FVector LaunchVelocity = KnockbackDirection.GetSafeNormal() * KnockbackPower;

    // 5. 캐릭터를 살짝 띄우기 위해 Z값을 추가합니다.
    LaunchVelocity.Z = 200.0f;

    UE_LOG(LogTemp, Warning, TEXT("HandleKnockback Re-calculated! LaunchVelocity: %s"), *LaunchVelocity.ToString());

    OwnerEnemyCharacter->LaunchCharacter(LaunchVelocity, true, true);
}

void UEnemyHitReactGameplayAbility::HandleLaunch()
{
    AEnemyCharacter* OwnerEnemyCharacter = GetEnemyCharacterFromActorInfo();
    if (nullptr == OwnerEnemyCharacter)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
    const FVector LaunchVelocity(0.0f, 0.0f, LaunchPower);
    OwnerEnemyCharacter->LaunchCharacter(LaunchVelocity, false, true);
}

bool UEnemyHitReactGameplayAbility::HandleSlamdown()
{
    AEnemyCharacter* OwnerEnemyCharacter = GetEnemyCharacterFromActorInfo();
    if (nullptr == OwnerEnemyCharacter || nullptr == OwnerEnemyCharacter->GetCharacterMovement())
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return false;
    }
    if (OwnerEnemyCharacter->GetCharacterMovement()->IsFalling())
    {
        const FVector LaunchVelocity(0.0f, 0.0f, -SlamdownPower);
        OwnerEnemyCharacter->LaunchCharacter(LaunchVelocity, false, true);
        return true;
    }
    return false;
}

void UEnemyHitReactGameplayAbility::OnMontageCompleted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyHitReactGameplayAbility::OnMontageBlendOut()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyHitReactGameplayAbility::OnMontageInterrupted()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEnemyHitReactGameplayAbility::OnMontageCancelled()
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}