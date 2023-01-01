// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class FPSSHOOTING_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly)
		class UArrowComponent* bulletArrow;
	UPROPERTY(EditDefaultsOnly, Category = Factory)
		TSubclassOf<class ABullet> bulletFactory;		//총알액터생성팩토리
	UPROPERTY(EditDefaultsOnly)
		class USoundBase* rifleFireSound;							//소총 사격 사운드

	void TurnToLeft();		//왼쪽으로 도는 애니메이션
	void TurnToRight();		//오른쪽으로 도는 애니메이션

	float walkSpeed = 200;
	bool detecting = false;

};
