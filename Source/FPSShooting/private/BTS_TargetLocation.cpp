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
	auto me = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (me == nullptr)
	{
		return;
	}
	float rotGap = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AEnemyAIController::TargetRotKey);
	if (rotGap > 10)
	{
		me->TurnToLeft();
	}
	else if (rotGap < -10)
	{
		me->TurnToRight();
	}
	else
	{
		me->GetMesh()->GetAnimInstance()->StopAllMontages(0);
	}
}
