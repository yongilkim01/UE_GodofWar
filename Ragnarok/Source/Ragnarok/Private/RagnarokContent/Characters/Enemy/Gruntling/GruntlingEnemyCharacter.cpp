// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/Gruntling/GruntlingEnemyCharacter.h"

#include "Components/CapsuleComponent.h"

AGruntlingEnemyCharacter::AGruntlingEnemyCharacter()
{
}

void AGruntlingEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

    float Scale = 1.5f;

    // 캡슐 크기 조정
    UCapsuleComponent* Capsule = GetCapsuleComponent();
    float NewRadius = Capsule->GetUnscaledCapsuleRadius() * Scale;
    float NewHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight() * Scale;
    Capsule->SetCapsuleSize(NewRadius, NewHalfHeight);

    // 메시 스케일 조정 (캡슐의 자식이면 Relative)
    GetMesh()->SetRelativeScale3D(FVector(Scale));

    // 메시 위치(오프셋)도 조정
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -NewHalfHeight));
}

void AGruntlingEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}