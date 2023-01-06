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
	virtual void OnPossess(APawn* InPawn) override;				//폰에 AIController를 바인딩
	UFUNCTION()		//시각으로 플레이어 발견 시 key값 부여
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
	UFUNCTION()		//청각으로 플레이어 발견 시 key값 부여
		void OnUpdated(TArray<AActor*> const& updatedActors);

	virtual void OnUnPossess() override;						//AIController 언바이딩

public:
	bool unPossessed = false;

	//AI Perception 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius = 3000.f;	//시야 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = AISightRadius + 300.f;		//시각 상실 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 60.f;		//시야 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 10.f;			//감각자극이 사라지는 시간

	static const FName HomePosKey;								//본인 위치
	static const FName PatrolPosKey;							//이동할 위치
	static const FName CanSeePlayerKey;							//플레이어를 찾았는가
	static const FName PlayerKey;								//플레이어 정보
	static const FName TargetLocKey;							//타겟 위치
	static const FName IsInvestigateKey;						//조사 해봐야 하는 상태
	static const FName TargetRotKey;							//본인과 플레이어간의 각도
	static const FName IsNarrowRotGapKey;						//본인이 타겟을 바라보았는가
};
