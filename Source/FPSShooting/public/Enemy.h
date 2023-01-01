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
		TSubclassOf<class ABullet> bulletFactory;		//�Ѿ˾��ͻ������丮
	UPROPERTY(EditDefaultsOnly)
		class USoundBase* rifleFireSound;							//���� ��� ����

	void TurnToLeft();		//�������� ���� �ִϸ��̼�
	void TurnToRight();		//���������� ���� �ִϸ��̼�

	float walkSpeed = 200;
	bool detecting = false;

};
