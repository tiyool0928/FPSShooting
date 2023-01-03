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

	FTimerHandle explosionTimerHandle;
	FTimerHandle deathTimerHandle;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;				//¼ö·ùÅº ¿Ü°ü
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* movementComp;	//ÀÌµ¿ ÄÄÆ÷³ÍÆ®

	void Explosion(); //Æø¹ß
	void Die();	//¹ß»çÃ¼ ¼Ò¸ê

	UPROPERTY(EditDefaultsOnly)
		class USoundBase* explosionSound;							//¼ö·ùÅº Æø¹ß »ç¿îµå
};
