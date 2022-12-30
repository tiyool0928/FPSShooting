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

	//플레이어 이동속도 참조
	auto ownerPawn = TryGetPawnOwner();
	auto me = Cast<AEnemy>(ownerPawn);
	//캐스팅 성공 시
	if (me)
	{
		//플레이어의 이동속도
		FVector velocity = me->GetVelocity();
		//플레잉어의 전방 벡터
		FVector forwardVector = me->GetActorForwardVector();
		//speed에 할당
		speed = FVector::DotProduct(forwardVector, velocity);
		//플레이어를 감지한 상태인가
		detecting = me->detecting;
	}
}
