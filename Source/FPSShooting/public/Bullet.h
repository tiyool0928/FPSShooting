// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class FPSSHOOTING_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle deathTimerHandle;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp;						//충돌체
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;				//총알 외관
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* movementComp;	//이동 컴포넌트

	void Die();	//발사체 소멸

	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
