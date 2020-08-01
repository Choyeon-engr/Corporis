// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "CorporisAIController.h"
#include "CorporisChampion.h"
#include "CorporisMinion.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CorporisAnimInstance.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
    NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory) const
{
    Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
    
    auto CorporisMinion = Cast<ACorporisMinion>(OwnerComp.GetAIOwner()->GetPawn());
    if (!CorporisMinion) { return false; }
    
    auto Target = Cast<ACorporisChampion>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACorporisAIController::TargetKey));
    if (!Target) { return false; }
    
    if (CorporisMinion->GetDistanceTo(Target) <= 2000.0f) { return true; }
    
    else
    {
        auto CorporisAnim = Cast<UCorporisAnimInstance>(CorporisMinion->GetMesh()->GetAnimInstance());
        CorporisAnim->SetAttackAngle(3);
        
        return false;
    }
}
