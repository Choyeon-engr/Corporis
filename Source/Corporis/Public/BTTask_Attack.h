// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Corporis.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 *
 */
UCLASS()
class CORPORIS_API UBTTask_Attack : public UBTTaskNode
{
    GENERATED_BODY()
    
public:
    UBTTask_Attack();
    
protected:
    virtual void TickTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    
public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory) override;
};
