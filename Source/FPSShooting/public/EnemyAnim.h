// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOOTING_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//����� ���� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* TurnLeftMontage;

	//����� ��� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* TurnRightMontage;

	void PlayTurnLeftMontage();
	void PlayTurnRightMontage();


	//�� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		float speed = 0;
	//�� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		bool detecting = false;

	//�� ������ ����
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
};
