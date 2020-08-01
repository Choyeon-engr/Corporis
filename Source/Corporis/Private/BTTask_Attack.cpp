// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "CorporisAIController.h"
#include "CorporisMinion.h"

UBTTask_Attack::UBTTask_Attack()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    
    return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    
    auto CorporisMinion = Cast<ACorporisMinion>(OwnerComp.GetAIOwner()->GetPawn());
    CorporisMinion->Attack();
    
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
