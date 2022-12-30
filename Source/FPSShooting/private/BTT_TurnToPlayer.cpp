// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurnToPlayer.h"
#include "EnemyAIController.h"
#include "Player1.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_TurnToPlayer::UBTT_TurnToPlayer()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTT_TurnToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto me = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (me == nullptr)
		return EBTNodeResult::Failed;
	auto Target = Cast<APlayer1>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::PlayerKey));
	if (nullptr == Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL Target"));
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - me->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	float rotGap = me->GetActorRotation().Yaw - TargetRot.Yaw;
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AEnemyAIController::TargetRotKey, rotGap);

	me->SetActorRotation(FMath::RInterpTo(me->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 5.0f));
	//UE_LOG(LogTemp, Warning, TEXT("rotGap: %.3f"), rotGap);

	if (rotGap <= 10)
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsNarrowRotGapKey, true);
	else if(rotGap >= -10)
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsNarrowRotGapKey, false);

	return EBTNodeResult::Succeeded;
}