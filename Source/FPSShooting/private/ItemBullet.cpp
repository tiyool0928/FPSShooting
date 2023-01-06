// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBullet.h"
#include "Components/BoxComponent.h"
#include "Player1.h"
#include "UI_Player.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AItemBullet::AItemBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(1, 3, 1));
	boxComp->SetWorldScale3D(FVector(10, 10, 10));
	//외관 컴포넌트
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);	//외관은 nocollision
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_556x45.SM_Shell_556x45'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		meshComp->SetRelativeLocation(FVector(0, -0.5, 0));
	}
}

// Called when the game starts or when spawned
void AItemBullet::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AItemBullet::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &AItemBullet::Die, 30, false);
}

// Called every frame
void AItemBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator nextRotation = GetActorRotation();
	nextRotation.Yaw += 20;

	boxComp->SetRelativeRotation(FMath::RInterpTo(GetActorRotation(),
		nextRotation, GetWorld()->GetDeltaSeconds(), 3.0f));
}

void AItemBullet::Die()
{
	GetWorld()->GetTimerManager().ClearTimer(deathTimerHandle);
	Destroy();
}

void AItemBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto Player = Cast<APlayer1>(OtherActor);

		if (OtherActor == Player)
		{
			int dropBullet = FMath::FRandRange(10, 31);
			Player->keepRifleBullet += dropBullet;

			if (IsValid(Player->playerWidget))	//UI 업데이트
			{
				Player->_playerWidget->UpdateAmmoBySwap();
			}
			Destroy();
		}
	}
}

