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
	//적 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		float speed = 0;

	//매 프레임 갱신
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
};
