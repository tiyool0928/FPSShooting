// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"

void UEnemyAnim::PlayTurnLeftMontage()
{
	if (TurnLeftMontage != nullptr)
		Montage_Play(TurnLeftMontage);
	//UE_LOG(LogTemp, Warning, TEXT("check : left"));
}

void UEnemyAnim::PlayTurnRightMontage()
{
	if (TurnRightMontage != nullptr)
		Montage_Play(TurnRightMontage);
	//UE_LOG(LogTemp, Warning, TEXT("check : right"));
	
}

void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//�÷��̾� �̵��ӵ� ����
	auto ownerPawn = TryGetPawnOwner();
	auto me = Cast<AEnemy>(ownerPawn);
	//ĳ���� ���� ��
	if (me)
	{
		//�÷��̾��� �̵��ӵ�
		FVector velocity = me->GetVelocity();
		//�÷��׾��� ���� ����
		FVector forwardVector = me->GetActorForwardVector();
		//speed�� �Ҵ�
		speed = FVector::DotProduct(forwardVector, velocity);
		//�÷��̾ ������ �����ΰ�
		detecting = me->detecting;
	}
}
