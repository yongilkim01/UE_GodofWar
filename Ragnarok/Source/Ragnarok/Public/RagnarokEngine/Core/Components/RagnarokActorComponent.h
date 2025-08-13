// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RagnarokActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RAGNAROK_API URagnarokActorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	template<class T>
	T* GetRagnarokOwner() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T', template parameter is not pawn must be check!!");
		return CastChecked<T>(GetOwner());
	}
	APawn* GetRagnarokOwner()
	{
		return GetRagnarokOwner<APawn>();
	}

	template <class T>
	T* GetRagnarokOwnerController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T', template parameter is not controller must be check!!");
		return GetRagnarokOwner<APawn>()->GetController<T>();
	}
	AController* GetRagnarokOwnerController()
	{
		return GetRagnarokOwnerController<AController>();
	}

protected:
	UPROPERTY(EditAnywhere, Category = "Ragnarok|Component")
	bool bShowDebugMsg = false;
};
