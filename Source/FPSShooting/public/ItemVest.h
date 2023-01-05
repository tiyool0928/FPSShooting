// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemVest.generated.h"

UCLASS()
class FPSSHOOTING_API AItemVest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemVest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle deathTimerHandle;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* boxComp;						//�浹ü
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;				//�Ƹ� �ܰ�

	void Die();	//�߻�ü �Ҹ�

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
