#include "BTService_Detect.h"
#include "CorporisAIController.h"
#include "CorporisChampion.h"
#include "CorporisMinion.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Detect::UBTService_Detect()
{
    NodeName = TEXT("Detect");
    Interval = 0.5f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
    auto CorporisMinion = Cast<ACorporisMinion>(OwnerComp.GetAIOwner()->GetPawn());
    if (!CorporisMinion) { return; }
    
    auto World = CorporisMinion->GetWorld();
    if (!World) { return; }
    
    FVector Center = CorporisMinion->GetActorLocation();
    float DetectRadius = 3000.0f;
    
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionQueryParam(NAME_None, false, CorporisMinion);
    bool bResult = World->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParam);
    
    OwnerComp.GetBlackboardComponent()->SetValueAsObject(ACorporisAIController::TargetKey, nullptr);
    
    if (bResult)
    {
        for (auto OverlapResult : OverlapResults)
        {
            auto Target = Cast<ACorporisChampion>(OverlapResult.GetActor());
            
            if (Target && Target->GetController()->IsPlayerController())
            {
                auto CorporisAnim = Cast<UCorporisAnimInstance>(CorporisMinion->GetMesh()->GetAnimInstance());
                
                if (Target->ChampionIsDead())
                {
                    CorporisAnim->SetAttackAngle(3);
                    return;
                }
                
                FHitResult HitResult;
                FCollisionQueryParams Params(NAME_None, false, CorporisMinion);
                bResult = World->LineTraceSingleByChannel(HitResult, Center, Target->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel1, Params);
                auto CorporisChampion = Cast<ACorporisChampion>(HitResult.Actor);
                
                if (bResult && !CorporisChampion)
                {
                    CorporisAnim->SetAttackAngle(3);
                    CorporisMinion->SetOnSeePawn(false);
                    CorporisMinion->SetOnHearNoise(false);
                    return;
                }
                
                if (CorporisMinion->GetOnSeePawn() || CorporisMinion->GetOnHearNoise())
                    OwnerComp.GetBlackboardComponent()->SetValueAsObject(ACorporisAIController::TargetKey, CorporisChampion);
                else
                    CorporisAnim->SetAttackAngle(3);
            }
        }
    }
    
    else
    {
        CorporisMinion->SetOnSeePawn(false);
        CorporisMinion->SetOnHearNoise(false);
    }
}
