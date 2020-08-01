// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "CorporisAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
    NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    
    auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!ControllingPawn) { return EBTNodeResult::Failed; }
    
    auto NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
    if (!NavSystem) { return EBTNodeResult::Failed; }
    
    FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ACorporisAIController::InitialPosKey);
    FNavLocation NextPatrol;
    
    if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1000.0f, NextPatrol))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(ACorporisAIController::PatrolPosKey, NextPatrol.Location);
        
        return EBTNodeResult::Succeeded;
    }
    
    return EBTNodeResult::Failed;
}
