// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "CorporisAIController.h"
#include "CorporisChampion.h"
#include "CorporisMinion.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
    NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    
    auto CorporisMinion = Cast<ACorporisMinion>(OwnerComp.GetAIOwner()->GetPawn());
    if (!CorporisMinion) { return EBTNodeResult::Failed; }
    
    auto Target = Cast<ACorporisChampion>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACorporisAIController::TargetKey));
    if (!Target) { return EBTNodeResult::Failed; }
    
    FVector LookVector = Target->GetActorLocation() - CorporisMinion->GetActorLocation();
    LookVector.Z = 0.0f;

    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
    CorporisMinion->SetActorRotation(FMath::RInterpTo(CorporisMinion->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 10.0f));
    
    return EBTNodeResult::Succeeded;
}
