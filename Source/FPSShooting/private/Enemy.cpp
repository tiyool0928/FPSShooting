// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAnim.h"
#include <Components/ArrowComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//�޽� ������Ʈ
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Enemy/Mesh/Ch44_nonPBR_UE.Ch44_nonPBR_UE'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//������Ʈ ��ġ (0, 0, -90) ȸ�� (0, -90, 0)
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	//�Ѿ� ���� ��ġ (Arrow)
	bulletArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletArrow"));
	bulletArrow->SetupAttachment(GetMesh());
	bulletArrow->SetRelativeLocationAndRotation(FVector(0, 70, 135), FRotator(0, 0, 90));

	//�ڿ������� ȸ��
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::TurnToLeft()
{
	auto anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	anim->PlayTurnLeftMontage();
}

void AEnemy::TurnToRight()
{
	auto anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	anim->PlayTurnRightMontage();
}

