// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "Player1.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnim::PlayJumpMontage()
{
	if (JumpMontage != nullptr)
		Montage_Play(JumpMontage);
}

void UPlayerAnim::PlayFireMontage()
{
	if (FireMontage != nullptr)
		Montage_Play(FireMontage);
}

void UPlayerAnim::PlayReadyGrenadeMontage()
{
	if (ReadyGrenadeMontage != nullptr)
		Montage_Play(ReadyGrenadeMontage);
}

void UPlayerAnim::PlayThrowGrenadeMontage()
{
	if (ThrowGrenadeMontage != nullptr)
		Montage_Play(ThrowGrenadeMontage);
}

void UPlayerAnim::PlayReloadMontage()
{
	if (ReloadMontage != nullptr)
		Montage_Play(ReloadMontage);
}

void UPlayerAnim::PlayHitReactionMontage()
{
	if (HitReactionMontage != nullptr)
		Montage_Play(HitReactionMontage);
}

void UPlayerAnim::PlayGrenadeHitReactionMontage()
{
	if (GrenadeHitReactionMontage != nullptr)
		Montage_Play(GrenadeHitReactionMontage);
		
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//플레이어 이동속도 참조
	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<APlayer1>(ownerPawn);
	//캐스팅 성공 시
	if (player)
	{
		//플레이어의 이동속도
		FVector velocity = player->GetVelocity();
		//플레이어가 앉아있는가
		isCrouch = player->bIsCrouched;
		//플레이어가 공중에 있는가
		isInAir = player->GetMovementComponent()->IsFalling();
		//수류탄을 들고있는가
		isSwap3 = player->bUsingGrenade;
		//플레이어의 전방 벡터
		FVector forwardVector = player->GetActorForwardVector();
		//speed에 할당
		speed = FVector::DotProduct(forwardVector, velocity);
		//좌우 속도 할당
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
		
		FRotator ControlRot = player->GetControlRotation();
		FRotator ActorRot = player->GetActorRotation();
		//control - actor로 정규화한 값을 pitch에 할당
		pitch = UKismetMathLibrary::NormalizedDeltaRotator(ControlRot, ActorRot).Pitch;
	}
}
