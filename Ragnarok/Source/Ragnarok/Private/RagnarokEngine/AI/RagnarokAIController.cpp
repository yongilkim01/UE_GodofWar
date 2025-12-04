// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokEngine/AI/RagnarokAIController.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ARagnarokAIController::ARagnarokAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{

}
