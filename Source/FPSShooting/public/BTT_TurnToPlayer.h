// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TurnToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOOTING_API UBTT_TurnToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_TurnToPlayer();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
