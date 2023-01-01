// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_TargetLocation.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "Player1.h"
#include "Bullet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Components/ArrowComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

UBTS_TargetLocation::UBTS_TargetLocation()
{
	NodeName = TEXT("TargetLocation");
	Interval = 1.0f;
}

void UBTS_TargetLocation::FireDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(FireDelayHandle);
	isFireDelay = false;
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

		if (!isFireDelay)
		{
			FRotator targetRot;		//타겟으로의 방향
			FTransform target;		//타겟으로의 트랜스폼
			FVector startPos = me->bulletArrow->GetComponentLocation();		//시작 위치
			auto Target = Cast<APlayer1>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::PlayerKey));
			//타겟 위치
			FVector endPos = Target->GetActorLocation();
			targetRot = UKismetMathLibrary::FindLookAtRotation(startPos, endPos);
			target = UKismetMathLibrary::MakeTransform(startPos, targetRot, FVector(1, 1, 1));

			GetWorld()->SpawnActor<ABullet>(me->bulletFactory, target);
			UGameplayStatics::SpawnSoundAtLocation(this, me->rifleFireSound, me->GetActorLocation());

			GetWorld()->GetTimerManager().SetTimer(FireDelayHandle, this, &UBTS_TargetLocation::FireDelay, 0.1f);
			isFireDelay = true;
		}
	}
}
