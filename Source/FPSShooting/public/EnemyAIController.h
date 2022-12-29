// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOOTING_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
		class UBlackboardData* BBAsset;
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBehaviorTreeComponent* BTComp;

	UPROPERTY()
		class UBlackboardComponent* BlackboardComp;
	UPROPERTY()
		class UAISenseConfig_Sight* SightConfig;

public:
	AEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;				//���� AIController�� ���ε�
	UFUNCTION()		//�÷��̾� �߰� �� key�� �ο�
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);

public:
	//AI Perception ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius = 500.f;	//�þ� �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = AISightRadius + 50.f;		//�ð� ��� �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 90.f;		//�þ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 5.f;			//�����ڱ��� ������� �ð�

	static const FName HomePosKey;								//���� ��ġ
	static const FName PatrolPosKey;							//�̵��� ��ġ
	static const FName CanSeePlayerKey;							//�÷��̾ ã�Ҵ°�
	static const FName PlayerKey;								//�÷��̾� ��ġ
};
