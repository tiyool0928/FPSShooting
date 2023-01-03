// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_StopInvestigate.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_StopInvestigate::UBTT_StopInvestigate(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Stop Investigating");
}

EBTNodeResult::Type UBTT_StopInvestigate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	cont->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::IsInvestigateKey, false);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
