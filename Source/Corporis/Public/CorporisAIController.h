// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Corporis.h"
#include "AIController.h"
#include "CorporisAIController.generated.h"

/**
 *
 */
UCLASS()
class CORPORIS_API ACorporisAIController : public AAIController
{
    GENERATED_BODY()
    
public:
    ACorporisAIController();
    
public:
    virtual void OnPossess(APawn* InPawn) override;
    
    static const FName InitialPosKey;
    static const FName PatrolPosKey;
    static const FName TargetKey;
    
    void StopAI();
    
private:
    UPROPERTY()
    class UBehaviorTree* BTAsset;
    
    UPROPERTY()
    class UBlackboardData* BBAsset;
};
