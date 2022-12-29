// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_TargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOOTING_API UBTS_TargetLocation : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_TargetLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
