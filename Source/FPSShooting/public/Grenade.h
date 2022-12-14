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
		class UStaticMeshComponent* meshComp;				//????ź ?ܰ?
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* movementComp;	//?̵? ??????Ʈ

	void Explosion(); //????
	void Die();	//?߻?ü ?Ҹ?

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* explosionEffect;			//????ź ???? ????Ʈ
	UPROPERTY(EditDefaultsOnly)
		class USoundBase* explosionSound;							//????ź ???? ??????
};
