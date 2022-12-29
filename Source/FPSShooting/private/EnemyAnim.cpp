// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//�÷��̾� �̵��ӵ� ����
	auto ownerPawn = TryGetPawnOwner();
	auto enemyBoss = Cast<AEnemy>(ownerPawn);
	//ĳ���� ���� ��
	if (enemyBoss)
	{
		//�÷��̾��� �̵��ӵ�
		FVector velocity = enemyBoss->GetVelocity();
		//�÷��׾��� ���� ����
		FVector forwardVector = enemyBoss->GetActorForwardVector();
		//speed�� �Ҵ�
		speed = FVector::DotProduct(forwardVector, velocity);
	}
}
