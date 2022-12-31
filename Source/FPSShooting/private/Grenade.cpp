// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "DrawDebugHelpers.h"
#include "Player1.h"
#include <Sound/SoundBase.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
AGrenade::AGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//외관 컴포넌트
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(meshComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);	//외관은 nocollision
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67.SM_G67'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		meshComp->SetSimulatePhysics(true);
		meshComp->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	}
	//이동 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	movementComp->SetUpdatedComponent(meshComp);
	movementComp->InitialSpeed = 1500;
	movementComp->MaxSpeed = 1500;
	movementComp->ProjectileGravityScale = 1;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;
	movementComp->Friction = 1;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &AGrenade::Die, 3, false);
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::Die()
{
	TArray<FHitResult> OutHits;
	FVector MyLocation = GetActorLocation();
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(500);
	DrawDebugSphere(GetWorld(), MyLocation, CollisionSphere.GetSphereRadius(), 50, FColor::Cyan, true);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, MyLocation, MyLocation, FQuat::Identity, ECC_WorldStatic, CollisionSphere);

	if (isHit)
	{
		for (const FHitResult HitResult : OutHits)
		{
			UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(HitResult.GetActor()->GetRootComponent());

			if (MeshComp)
			{
				MeshComp->AddRadialImpulse(MyLocation, 500, 500, ERadialImpulseFalloff::RIF_Constant, true);
				UGameplayStatics::SpawnSoundAtLocation(this, explosionSound, GetActorLocation());
			}
		}
	}

	Destroy();
}

