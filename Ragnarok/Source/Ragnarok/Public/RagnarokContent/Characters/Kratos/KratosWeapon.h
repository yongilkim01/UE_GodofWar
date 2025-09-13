// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RagnarokEngine/Objects/Items/Weapons/RagnarokWeapon.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "KratosWeapon.generated.h"

class AKratos;
class UProjectileMovementComponent;
class UTimelineComponent;

/**
 * 크레토스 무기 베이스 클래스
 * 크레토스가 사용할 수 있는 모든 무기의 기본이 되는 클래스로 게임플레이 어빌리티와 투사체 기능을 제공한다
 * 
 * Kratos weapon base class.
 * Base class for all weapons that Kratos can use, providing gameplay ability and projectile functionality.
 */
UCLASS()
class RAGNAROK_API AKratosWeapon : public ARagnarokWeapon
{
	GENERATED_BODY()

public:
	/** 생성자 */
	AKratosWeapon();

	/** 무기를 초기화하는 함수 */
	virtual void InitWeapon() override;

	/**
	 * 부여받은 어빌리티 스펙 핸들을 할당하는 함수
	 *
	 * @param InSpecHandleArray 할당할 어빌리티 스펙 핸들 배열
	 */
	void AssignGratnAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandleArray);
	
	/**
	 * 무기의 소유자를 설정하는 함수
	 *
	 * @param RagnarokCharacter 무기를 소유할 라그나로크 캐릭터
	 */
	virtual void SetOwner(ARagnarokCharacter* RagnarokCharacter) override;
	
	/** 부여받은 어빌리티 스펙 핸들 배열을 반환하는 함수 */
	TArray<FGameplayAbilitySpecHandle>& GetGrantedAbilitySpecHandleArray();
	
	/**
	 * 무기를 던지는 함수 (가상 함수 - 하위 클래스에서 구현)
	 *
	 * @param CameraRotation 카메라 회전값
	 * @param CameraLocation 카메라 위치
	 * @param CameraForwardVector 카메라 전방 벡터
	 */
	virtual void ThrowWeapon(FRotator CameraRotation, FVector CameraLocation, FVector CameraForwardVector) {}
	
	/** 무기를 회수하는 함수 (가상 함수 - 하위 클래스에서 구현) */
	virtual void RecallWeapon() {}
	
	/** 무기를 정지시키는 함수 (가상 함수 - 하위 클래스에서 구현) */
	virtual void StopWeapon() {}
	
	/** 무기 트레일을 시작하는 함수 (가상 함수 - 하위 클래스에서 구현) */
	virtual void StartWeaponTrail() {}
	
	/** 무기 트레일을 종료하는 함수 (가상 함수 - 하위 클래스에서 구현) */
	virtual void EndWeaponTrail() {}

	/**
	 * 무기를 장착 해제하는 함수
	 *
	 * @param OwnerActor 무기를 소유한 액터
	 * @param SocketName 무기가 부착된 소켓 이름
	 */
	void UnUequipWeapon(const AActor* OwnerActor, FName SocketName);

protected:
	//~ Begin AActor Interface.
	
	/** 게임 시작 시 호출됨 */
	virtual void BeginPlay() override;
	
	/** 매 프레임 호출됨 */
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	//~ Begin ARagnarokWeapon Interface.
	
	/** 무기 데이터 에셋을 로드하는 함수 */
	virtual void LoadWeaponDataAsset() override;
	
	/**
	 * 무기 프라이머리 데이터 에셋을 로드하는 함수
	 *
	 * @param PDAAssetObject 로드할 프라이머리 데이터 에셋 오브젝트
	 */
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject) override;
	//~ End ARagnarokWeapon Interface.

private:
	/**
	 * 액터로부터 애니메이션 클래스 레이어를 연결 해제하는 함수
	 *
	 * @param UnlinkActor 연결 해제할 액터
	 */
	void UnlinkAnimClassLayersFromActor(const AActor* UnlinkActor);

private:
	/** 부여받은 어빌리티 스펙 핸들 배열 */
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandleArray;

protected:
	/** 투사체 움직임 컴포넌트 */
	UPROPERTY(EditAnywhere, Category = "KratosWeapon|Components", meta = (DisplayName = "Projectile Movement Component"))
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	/** 크레토스 무기 프라이머리 데이터 에셋 */
	UPROPERTY(EditAnywhere, Category = "KratosWeapon|Data", meta = (DisplayName = "KratosWeapon Primary Data Aaset"))
	UItemPrimaryAssetKratosWeapon* KratosWeaponPDA = nullptr;

	/** 무기를 소유한 크레토스 캐릭터에 대한 약한 참조 */
	UPROPERTY()
	TWeakObjectPtr<AKratos> OwnerKratos;

public:
	/** 무기 데이터 구조체 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KratosWeapon|Data", meta = (DisplayName = "Weapon Data"))
	FKratosWeaponData WeaponData;
};
