// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Player1.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	boxComp->SetBoxExtent(FVector(3, 1, 1));
	//외관 컴포넌트
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);	//외관은 nocollision
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_545x39.SM_Shell_545x39'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		meshComp->SetRelativeLocationAndRotation(FVector(-0.5f, 0, 0), FRotator(0, -90, 0));
	}
	//이동 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	movementComp->SetUpdatedComponent(boxComp);
	movementComp->InitialSpeed = 3000;
	movementComp->MaxSpeed = 3000;
	movementComp->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &ABullet::Die, 10, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die()
{
	Destroy();
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto otherActor = Cast<APlayer1>(OtherActor);

		if (OtherActor != otherActor)
			UGameplayStatics::ApplyDamage(OtherActor, 300.0f, nullptr, this, nullptr);

		Destroy();
	}
}

