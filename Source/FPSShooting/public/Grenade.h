// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class FPSSHOOTING_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle deathTimerHandle;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;				//총알 외관
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* movementComp;	//이동 컴포넌트

	void Die();	//발사체 소멸
};
