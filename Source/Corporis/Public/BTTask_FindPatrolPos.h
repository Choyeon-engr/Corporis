#pragma once

#include "Corporis.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

UCLASS()
class CORPORIS_API UBTTask_FindPatrolPos : public UBTTaskNode
{
    GENERATED_BODY()
    
public:
    UBTTask_FindPatrolPos();
    
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory) override;
};
