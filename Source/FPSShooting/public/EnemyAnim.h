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
	//�� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		float speed = 0;

	//�� ������ ����
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
};
