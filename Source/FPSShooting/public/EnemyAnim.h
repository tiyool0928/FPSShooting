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

	//����� �ǰ� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* HitReactionMontage;

	//����� ��� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* DieMontage;

	//����� �ȴٰ� ��� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* WalkingDieMontage;

	void PlayTurnLeftMontage();
	void PlayTurnRightMontage();
	void PlayHitReactionMontage();
	void PlayWalkingDieMontage();
	void PlayDieMontage();


	//�� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		float speed = 0;
	//�� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		bool detecting = false;

	//���� �����ΰ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool isDeath = false;

	//�� ������ ����
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
};
