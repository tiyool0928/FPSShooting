// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_StopInvestigate.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOOTING_API UBTT_StopInvestigate : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_StopInvestigate(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
