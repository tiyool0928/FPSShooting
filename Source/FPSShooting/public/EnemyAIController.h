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
	UPROPERTY()
		class UAISenseConfig_Hearing* HearingConfig;

public:
	AEnemyAIController(FObjectInitializer const& object_initializer);
	virtual void OnPossess(APawn* InPawn) override;				//���� AIController�� ���ε�
	UFUNCTION()		//�ð����� �÷��̾� �߰� �� key�� �ο�
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
	UFUNCTION()		//û������ �÷��̾� �߰� �� key�� �ο�
		void OnUpdated(TArray<AActor*> const& updatedActors);

	virtual void OnUnPossess() override;						//AIController ����̵�

public:
	bool unPossessed = false;

	//AI Perception ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius = 3000.f;	//�þ� �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = AISightRadius + 300.f;		//�ð� ��� �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 60.f;		//�þ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 10.f;			//�����ڱ��� ������� �ð�

	static const FName HomePosKey;								//���� ��ġ
	static const FName PatrolPosKey;							//�̵��� ��ġ
	static const FName CanSeePlayerKey;							//�÷��̾ ã�Ҵ°�
	static const FName PlayerKey;								//�÷��̾� ����
	static const FName TargetLocKey;							//Ÿ�� ��ġ
	static const FName IsInvestigateKey;						//���� �غ��� �ϴ� ����
	static const FName TargetRotKey;							//���ΰ� �÷��̾�� ����
	static const FName IsNarrowRotGapKey;						//������ Ÿ���� �ٶ󺸾Ҵ°�
};
