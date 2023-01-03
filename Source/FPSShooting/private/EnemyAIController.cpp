// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Player1.h"
#include "Enemy.h"
#include "Grenade.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"

const FName AEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName AEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyAIController::CanSeePlayerKey(TEXT("CanSeePlayer"));
const FName AEnemyAIController::PlayerKey(TEXT("Target"));
const FName AEnemyAIController::TargetLocKey(TEXT("TargetLoc"));
const FName AEnemyAIController::IsInvestigateKey(TEXT("IsInvestigate"));
const FName AEnemyAIController::IsNarrowRotGapKey(TEXT("IsNarrowRotGap"));
const FName AEnemyAIController::TargetRotKey(TEXT("TargetRot"));

AEnemyAIController::AEnemyAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Enemy/BB_Enemy.BB_Enemy'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Enemy/BT_Enemy.BT_Enemy'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	BTComp = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	//AI Perception
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	//시각 센서
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	//청각 센서
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	HearingConfig->HearingRange = 1500.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;
	HearingConfig->SetMaxAge(AISightAge);

	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnUpdated);
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void AEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (auto const player = Cast<APlayer1>(actor))
	{
		//플레이어를 감지하면 true값을 넣어준다.
		BlackboardComp->SetValueAsBool(TEXT("CanSeePlayer"), Stimulus.WasSuccessfullySensed());
		BlackboardComp->SetValueAsObject(TEXT("Target"), player);
		auto me = Cast<AEnemy>(BTComp->GetAIOwner()->GetPawn());

		if (Stimulus.WasSuccessfullySensed())
		{
			me->detecting = true;
			UE_LOG(LogTemp, Warning, TEXT("true"));
		}
		else
		{
			me->detecting = false;
			me->GetMesh()->GetAnimInstance()->StopAllMontages(0);
			UE_LOG(LogTemp, Warning, TEXT("false"));
		}
	}
}

void AEnemyAIController::OnUpdated(TArray<AActor*> const& updatedActors)
{
	UE_LOG(LogTemp, Warning, TEXT("check"));
	for (AActor* DetectedPawn : updatedActors)
	{
		if (!HearingConfig->GetSenseID().IsValid()) return;
		auto me = Cast<AEnemy>(BTComp->GetAIOwner()->GetPawn());
		FActorPerceptionBlueprintInfo info;
		GetPerceptionComponent()->GetActorsPerception(DetectedPawn, info);
		for (size_t i = 0; i < info.LastSensedStimuli.Num(); ++i)
		{
			FAIStimulus const stim = info.LastSensedStimuli[i];
			if(stim.IsValid())
				UE_LOG(LogTemp, Warning, TEXT("SensingComplete"));
			//감지 감각이 청각이면
			if (stim.Tag == TEXT("Noise"))
			{
				//UE_LOG(LogTemp, Warning, TEXT("HI"));
				BlackboardComp->SetValueAsBool(TEXT("IsInvestigate"), stim.WasSuccessfullySensed());
				BlackboardComp->SetValueAsVector(TEXT("TargetLoc"), stim.StimulusLocation);
			}
			//아니라면 시각
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Bye"));
				BlackboardComp->SetValueAsBool(TEXT("CanSeePlayer"), stim.WasSuccessfullySensed());
			}
		}
	}
}
