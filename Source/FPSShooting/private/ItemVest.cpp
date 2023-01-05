// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemVest.h"
#include "Components/BoxComponent.h"
#include "Player1.h"
#include "UI_Player.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AItemVest::AItemVest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//충돌체 컴포넌트
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(320, 320, 320));
	//외관 컴포넌트
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);	//외관은 nocollision
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/Enemy/Mesh/Item/uploads_files_2060891_Flak+Jacket.uploads_files_2060891_Flak+Jacket'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AItemVest::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AItemVest::OnOverlapBegin);
	GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, this, &AItemVest::Die, 30, false);
}

// Called every frame
void AItemVest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator nextRotation = GetActorRotation();
	nextRotation.Yaw += 20;

	boxComp->SetRelativeRotation(FMath::RInterpTo(GetActorRotation(),
		nextRotation, GetWorld()->GetDeltaSeconds(), 3.0f));
}

void AItemVest::Die()
{
	GetWorld()->GetTimerManager().ClearTimer(deathTimerHandle);
	Destroy();
}

void AItemVest::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		auto Player = Cast<APlayer1>(OtherActor);

		if (OtherActor == Player)
		{
			if (Player->playerArmor + 20 > 100)
			{
				Player->playerArmor = 100;
			}
			else
			{
				Player->playerArmor += 20;
			}
			if (IsValid(Player->playerWidget))	//UI 업데이트
			{
				Player->_playerWidget->UpdateArmorBar();
			}
			Destroy();
		}
	}
}

