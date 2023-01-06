// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAnim.h"
#include "EnemyAIController.h"
#include "ItemVest.h"
#include "ItemBullet.h"
#include <Components/ArrowComponent.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//메시 컴포넌트
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Enemy/Mesh/Ch44_nonPBR_UE.Ch44_nonPBR_UE'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		//컴포넌트 위치 (0, 0, -90) 회전 (0, -90, 0)
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	//총알 생성 위치 (Arrow)
	bulletDir = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletArrow"));
	bulletDir->SetupAttachment(GetMesh());
	bulletDir->SetRelativeLocationAndRotation(FVector(0, 70, 135), FRotator(0, 90, 0));

	//자연스러운 회전
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

void AEnemy::Die()
{
	AEnemyAIController* AIController = this->GetController<AEnemyAIController>();

	if (AIController != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnPossess"));
		AIController->OnUnPossess();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Didn't UnPossess"));
	}

	auto anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	anim->isDeath = true;
	if (detecting)
		anim->PlayDieMontage();
	else
		anim->PlayWalkingDieMontage();

	int RandItem = FMath::FRandRange(0, 2);
	UE_LOG(LogTemp, Warning, TEXT("%d"), RandItem);
	if(RandItem == 0)
		GetWorld()->SpawnActor<AItemVest>(itemVestFactory, GetActorTransform());
	else
		GetWorld()->SpawnActor<AItemBullet>(itemBulletFactory, GetActorTransform());

	GetWorld()->GetTimerManager().SetTimer(DieTimerHandle, this, &AEnemy::DestroyTemp, 3.0f, false);
	//GetWorld()->GetFirstPlayerController()->SetPause(true);
}

void AEnemy::DestroyTemp()
{
	GetWorld()->GetTimerManager().ClearTimer(DieTimerHandle);
	Destroy();
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		for (auto bName : criticalBone)
		{
			if (PointDamageEvent->HitInfo.BoneName == bName)
			{
				if (bName == "head")
					ActualDamage *= 3; // 맞은 부위가 머리면, 데미지 3배
				else
					ActualDamage *= 2; // 맞은 부위가 치명부위이면, 데미지 2배
			}
		}
		FVector impactLocation = PointDamageEvent->HitInfo.ImpactPoint;
		UE_LOG(LogTemp, Warning, TEXT("BoneName :: %s"), *PointDamageEvent->HitInfo.BoneName.ToString());
	}
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);
	}
	
	auto anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	if (ActualDamage > 0.0f)
	{
		health -= ActualDamage;
		anim->PlayHitReactionMontage();
		UE_LOG(LogTemp, Warning, TEXT("EnemyHealth: %f"), health);

	}
	if (health <= 0)
	{
		SetActorEnableCollision(false);
		Die();
	}

	return ActualDamage;
}

