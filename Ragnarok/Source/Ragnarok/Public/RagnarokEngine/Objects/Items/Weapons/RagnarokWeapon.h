// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RagnarokContent/Core/Types/RagnarokContentTypes.h"
#include "RagnarokWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UItemPrimaryAssetKratosWeapon;
class ARagnarokCharacter;

struct FAttachmentTransformRules;

/** 무기 초기화 완료 시 호출되는 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponInitialized);

/** 타겟과 상호작용 시 호출되는 델리게이트 */
DECLARE_DELEGATE_OneParam(FOnTargetInteractedDelegate, AActor*);

/**
 * 라그나로크 무기 베이스 클래스
 * 게임 내 모든 무기의 기본이 되는 클래스로 메시, 충돌, 초기화 등의 기본 기능을 제공한다
 * 
 * Ragnarok weapon base class.
 * Base class for all weapons in the game, providing basic functionality like mesh, collision, and initialization.
 */
UCLASS()
class RAGNAROK_API ARagnarokWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	/** 생성자 */
	ARagnarokWeapon();

	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//~ End AActor Interface.

	/** 무기를 초기화하는 함수 (가상 함수 - 하위 클래스에서 구현) */
	virtual void InitWeapon() {}
	
	/**
	 * 무기의 소유자를 설정하는 함수 (가상 함수 - 하위 클래스에서 구현)
	 *
	 * @param RagnarokCharacter 무기를 소유할 라그나로크 캐릭터
	 */
	virtual void SetOwner(ARagnarokCharacter* RagnarokCharacter) {};

	/**
	 * 무기를 액터에 부착하는 함수
	 *
	 * @param ActorToAttach 무기를 부착할 액터
	 * @param AttachmentRules 부착 트랜스폼 규칙
	 * @param SocketName 부착할 소켓 이름
	 */
	void AttachWeaponToActor(const AActor* ActorToAttach, const FAttachmentTransformRules& AttachmentRules, FName SocketName);

	// Getters (Inline Functions)
	/** 무기 메시를 반환하는 함수 */
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	/** 무기 충돌 컴포넌트를 반환하는 함수 */
	FORCEINLINE UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
	/** 무기 초기화 여부를 반환하는 함수 */
	FORCEINLINE bool IsInitialize() const { return bInitialized; }
	/** 현재 무기 상태를 반환하는 함수 */
	FORCEINLINE ERagnarokWeaponState GetWeaponState() const { return CurWeaponState; }

	// Public Variables (BlueprintAssignable Delegates)
	/** 무기 초기화 완료 시 브로드캐스트되는 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "RagnarokWeapon|Events", meta = (DisplayName = "On Weapon Initialized"))
	FOnWeaponInitialized OnWeaponInitialized;

	/** 무기가 타겟에 충돌했을 때 호출되는 델리게이트 */
	FOnTargetInteractedDelegate OnWeaponHitTarget;
	/** 무기가 타겟으로부터 분리될 때 호출되는 델리게이트 */
	FOnTargetInteractedDelegate OnWeaponPulledFromTarget;

protected:
	/** 무기 데이터 에셋을 로드하는 함수 (가상 함수 - 하위 클래스에서 구현) */
	virtual void LoadWeaponDataAsset();
	
	/**
	 * 무기 프라이머리 데이터 에셋을 로드하는 함수 (가상 함수 - 하위 클래스에서 구현)
	 *
	 * @param PDAAssetObject 로드할 프라이머리 데이터 에셋 오브젝트
	 */
	virtual void LoadWeaponPrimaryDataAsset(UObject* PDAAssetObject);

	/**
	 * 충돌 박스 오버랩 시작 시 호출되는 함수
	 *
	 * @param OverlappedComponent 오버랩된 컴포넌트
	 * @param OtherActor 충돌한 다른 액터
	 * @param OtherComp 충돌한 다른 컴포넌트
	 * @param OtherBodyIndex 다른 바디 인덱스
	 * @param bFromSweep 스윕으로부터 발생한 충돌인지 여부
	 * @param SweepResult 스윕 결과
	 */
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(
		UPrimitiveComponent*OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/**
	 * 충돌 박스 오버랩 종료 시 호출되는 함수
	 *
	 * @param OverlappedComponent 오버랩된 컴포넌트
	 * @param OtherActor 충돌한 다른 액터
	 * @param OtherComp 충돌한 다른 컴포넌트
	 * @param OtherBodyIndex 다른 바디 인덱스
	 */
	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// Protected Variables (UPROPERTY for Editor/Blueprint access)
	/** 루트 씬 컴포넌트 */
	UPROPERTY(EditAnywhere, Category = "RagnarokWeapon|Components", meta = (DisplayName = "Root Scene Component"))
	USceneComponent* RootSceneComponent = nullptr;

	/** 무기 스켈레탈 메시 컴포넌트 */
	UPROPERTY(EditAnywhere, Category = "RagnarokWeapon|Components", meta = (DisplayName = "Weapon Mesh"))
	USkeletalMeshComponent* WeaponMesh = nullptr;

	/** 무기 충돌 박스 컴포넌트 */
	UPROPERTY(EditAnywhere, Category = "RagnarokWeapon|Components", meta = (DisplayName = "Weapon Collision"))
	UBoxComponent* WeaponCollision = nullptr;

	/** 무기 프라이머리 데이터 에셋 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RagnarokWeapon|Data", meta = (DisplayName = "Weapon Primary Data Asset"))
	UItemPrimaryAssetKratosWeapon* WeaponPDA = nullptr;

	/** 무기 프라이머리 데이터 에셋 ID */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RagnarokWeapon|Data", meta = (DisplayName = "Weapon PDA ID"))
	FPrimaryAssetId WeaponPDAId;

	/** 무기 초기화 완료 여부 */
	bool bInitialized = false;
	
	/** 현재 무기 상태 */
	ERagnarokWeaponState CurWeaponState = ERagnarokWeaponState::ERWS_None;
};
