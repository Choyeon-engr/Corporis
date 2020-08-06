#include "CorporisAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"

const FName ACorporisAIController::InitialPosKey(TEXT("InitialPos"));
const FName ACorporisAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ACorporisAIController::TargetKey(TEXT("Target"));

ACorporisAIController::ACorporisAIController()
{
    static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/ParagonMinions/BB_CorporisAI.BB_CorporisAI"));
    if (BBObject.Succeeded()) { BBAsset = BBObject.Object; }
    
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/ParagonMinions/BT_CorporisAI.BT_CorporisAI"));
    if (BTObject.Succeeded()) { BTAsset = BTObject.Object; }
}

void ACorporisAIController::StopAI()
{
    auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
}

void ACorporisAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    UseBlackboard(BBAsset, Blackboard);
    Blackboard->SetValueAsVector(InitialPosKey, InPawn->GetActorLocation());
    RunBehaviorTree(BTAsset);
}
