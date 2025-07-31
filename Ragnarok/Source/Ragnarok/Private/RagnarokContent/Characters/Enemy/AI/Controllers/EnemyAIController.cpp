// Fill out your copyright notice in the Description page of Project Settings.


#include "RagnarokContent/Characters/Enemy/AI/Controllers/EnemyAIController.h"
#include "RagnarokEngine/Kismet/Debug/RagnarokDebugHelper.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))

{
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfigSight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 5000.0f;
	AISenseConfig_Sight->LoseSightRadius = 0.0f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.0f;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponAent"));
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AEnemyAIController::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	UCrowdFollowingComponent* CrowdFollowingComponent = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());

	if (nullptr != CrowdFollowingComponent)
	{
		CrowdFollowingComponent->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);
		
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);    break;
		case 2: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);   break;
		case 4: CrowdFollowingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);   break;
		default: break;
		}

		CrowdFollowingComponent->SetAvoidanceGroup(1);
		CrowdFollowingComponent->SetGroupsToAvoid(1);
		CrowdFollowingComponent->SetCrowdCollisionQueryRange(CrowdCollisionQueryRange);
	}

}

ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	
	const IGenericTeamAgentInterface* TeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (nullptr != TeamAgent && GetGenericTeamId() > TeamAgent->GetGenericTeamId())
	{
		// �� ���̵� ���� �ٸ��ٸ� ���� ���踦 ��ȯ.
		return ETeamAttitude::Hostile;
	}

	// ���� ���谡 �ƴ϶�� ���� ���� ���踦 ��ȯ.
	return ETeamAttitude::Friendly;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (nullptr != BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);

		if (ACharacter* InCharacter = Cast<ACharacter>(InPawn))
		{
			GetBlackboardComponent()->SetValueAsFloat(FName("DefaultMaxWalkSpeed"), InCharacter->GetCharacterMovement()->MaxWalkSpeed);

		}
	}
}

void AEnemyAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (nullptr == BlackboardComponent->GetValueAsObject("TargetActor"))
		{
			if (true == Stimulus.WasSuccessfullySensed() && nullptr != Actor)
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}
}
