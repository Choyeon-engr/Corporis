// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Corporis.h"
#include "Animation/AnimInstance.h"
#include "CorporisAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FCEnablePhysicsDelegate);
DECLARE_MULTICAST_DELEGATE(FMEnablePhysicsDelegate);

/**
 *
 */
UCLASS()
class CORPORIS_API UCorporisAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
    
public:
    UCorporisAnimInstance();
    
public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    
    void SetIsDead(bool IsDead) { bIsDead = IsDead; }
    void SetAttackAngle(int AttackAngle) { iAttackAngle = AttackAngle; }
    void SetIsSit(bool IsSit) { bIsSit = IsSit; }
    void SetIsDamaged(bool IsDamaged) { bIsDamaged = IsDamaged; }
    
    FCEnablePhysicsDelegate CEnablePhysics;
    FMEnablePhysicsDelegate MEnablePhysics;
    
private:
    UFUNCTION()
    void AnimNotify_CEnablePhysics() { CEnablePhysics.Broadcast(); }
    
    UFUNCTION()
    void AnimNotify_MEnablePhysics() { MEnablePhysics.Broadcast(); }
    
    UFUNCTION()
    void AnimNotify_CFinishHitReact() { bIsDamaged = false; }
    
    UFUNCTION()
    void AnimNotify_MFinishHitReact() { bIsDamaged = false; }
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
    float CurrentPawnSpeed;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    bool bIsDead;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    int iAttackAngle;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    bool bIsSit;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Status, Meta = (AllowPrivateAccess = true))
    bool bIsDamaged;
};
