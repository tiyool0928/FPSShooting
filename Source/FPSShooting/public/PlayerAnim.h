// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOOTING_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//플레이어 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float speed = 0;
	//플레이어 좌우 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float direction = 0;
	//플레이어 시선 (위아래)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float pitch = 0;
	//플레이어가 앉아있는지
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool isCrouch = false;

	//재생할 점프 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* JumpMontage;

	//재생할 사격 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* FireMontage;

	void PlayJumpMontage();
	void PlayFireMontage();


	//매 프레임 갱신
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
