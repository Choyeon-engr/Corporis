#pragma once

#include "Corporis.h"
#include "AIController.h"
#include "CorporisAIController.generated.h"

UCLASS()
class CORPORIS_API ACorporisAIController : public AAIController
{
    GENERATED_BODY()
    
public:
    ACorporisAIController();
    
    void StopAI();
    
protected:
    virtual void OnPossess(APawn* InPawn) override;
    
public:
    static const FName InitialPosKey;
    static const FName PatrolPosKey;
    static const FName TargetKey;
    
private:
    UPROPERTY()
    class UBehaviorTree* BTAsset;
    
    UPROPERTY()
    class UBlackboardData* BBAsset;
};
