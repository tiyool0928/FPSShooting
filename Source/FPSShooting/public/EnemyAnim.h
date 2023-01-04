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
	//재생할 점프 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* TurnLeftMontage;

	//재생할 사격 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* TurnRightMontage;

	//재생할 피격 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* HitReactionMontage;

	//재생할 사망 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* DieMontage;

	//재생할 걷다가 사망 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* WalkingDieMontage;

	void PlayTurnLeftMontage();
	void PlayTurnRightMontage();
	void PlayHitReactionMontage();
	void PlayWalkingDieMontage();
	void PlayDieMontage();


	//적 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		float speed = 0;
	//적 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayAnim)
		bool detecting = false;

	//죽은 상태인가
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool isDeath = false;

	//매 프레임 갱신
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
		
};
