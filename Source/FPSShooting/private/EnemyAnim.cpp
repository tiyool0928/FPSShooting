// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//플레이어 이동속도 참조
	auto ownerPawn = TryGetPawnOwner();
	auto enemyBoss = Cast<AEnemy>(ownerPawn);
	//캐스팅 성공 시
	if (enemyBoss)
	{
		//플레이어의 이동속도
		FVector velocity = enemyBoss->GetVelocity();
		//플레잉어의 전방 벡터
		FVector forwardVector = enemyBoss->GetActorForwardVector();
		//speed에 할당
		speed = FVector::DotProduct(forwardVector, velocity);
	}
}
