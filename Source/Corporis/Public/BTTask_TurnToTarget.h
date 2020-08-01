// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Corporis.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget.generated.h"

/**
 *
 */
UCLASS()
class CORPORIS_API UBTTask_TurnToTarget : public UBTTaskNode
{
    GENERATED_BODY()
    
public:
    UBTTask_TurnToTarget();
    
public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory) override;
};
