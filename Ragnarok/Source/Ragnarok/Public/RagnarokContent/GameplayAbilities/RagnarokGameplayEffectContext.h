// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "RagnarokGameplayEffectContext.generated.h"

/**
 * 공격 타입, 피격 방향을 담기 위한 GameplayEffectContext 구조체
 */
USTRUCT()
struct RAGNAROK_API FRagnarokGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
		
public:
	FRagnarokGameplayEffectContext() : FGameplayEffectContext() {}

	FGameplayTag GetAttackTypeTag() const { return AttackTypeTag; }
	void SetAttackTypeTag(FGameplayTag Tag) { AttackTypeTag = Tag; }

	FVector_NetQuantizeNormal GetHitDirection() const { return HitDirection; }
	void SetHitDirection(FVector_NetQuantizeNormal Direction) { HitDirection = Direction; }

protected:
	UPROPERTY()
	FGameplayTag AttackTypeTag;

	UPROPERTY()
	FVector_NetQuantizeNormal HitDirection;

public:
	/** 
	 * 필수 오버라이드 함수 
	 * 언리얼 리플렉션 시스템이 런타임에 실제 구조체 타입을 식별할 수 있게 만드는 메소드
	 */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FRagnarokGameplayEffectContext::StaticStruct();
	}
	/**
	 * 필수 오버라이드 함수
	 * FRagnarokGameplayEffectContext의 깊은 복사본을 생성하여 반환하는 함수
	 */
	virtual FRagnarokGameplayEffectContext* Duplicate() const override
	{
		FRagnarokGameplayEffectContext* NewContext = new FRagnarokGameplayEffectContext();
		*NewContext = *this;
		NewContext->AddActors(Actors);
		if (GetHitResult())
		{
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	/**
	 * 필수 오버라이드 함수
	 * 네트워크 복제를 위한 직렬화 함수
	 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSucess) override;

};
/**
 * 언리얼 리플렉션 시스템 및 네트워크 시스템에게 이 구조체의 특성을 정의
 * WithNetSerializer = true : 커스텀 NetSerializer 함수를 사용한다고 명시
 * WithCopy = true : 복사가 가능함을 명시
 */
template<>
struct TStructOpsTypeTraits<FRagnarokGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FRagnarokGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};