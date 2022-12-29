// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_TargetLocation.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "Player1.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_TargetLocation::UBTS_TargetLocation()
{
	NodeName = TEXT("TargetLocation");
	Interval = 1.0f;
}

void UBTS_TargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto target = Cast<APlayer1>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::PlayerKey));
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("TargetPos"), target->GetActorLocation());
}
